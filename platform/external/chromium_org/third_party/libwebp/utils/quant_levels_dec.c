// Copyright 2013 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#include "./quant_levels_dec.h"

#include <string.h>   

#include "./utils.h"


#define FIX 16     
#define LFIX 2     
#define LUT_SIZE ((1 << (8 + LFIX)) - 1)  

#if defined(USE_DITHERING)

#define DFIX 4           
#define DSIZE 4          
static const uint8_t kOrderedDither[DSIZE][DSIZE] = {
 {  0,  8,  2, 10 },     
 { 12,  4, 14,  6 },
 {  3, 11,  1,  9 },
 { 15,  7, 13,  5 }
};

#else
#define DFIX 0
#endif

typedef struct {
  int width_, height_;  
  int row_;             
  uint8_t* src_;        
  uint8_t* dst_;        

  int radius_;          
  int scale_;           

  void* mem_;           

  
  uint16_t* start_;
  uint16_t* cur_;
  uint16_t* end_;
  uint16_t* top_;
  uint16_t* average_;

  
  int num_levels_;       
  int min_, max_;        
  int min_level_dist_;   

  int16_t* correction_;  
} SmoothParams;


#define CLIP_MASK (int)(~0U << (8 + DFIX))
static WEBP_INLINE uint8_t clip_8b(int v) {
  return (!(v & CLIP_MASK)) ? (uint8_t)(v >> DFIX) : (v < 0) ? 0u : 255u;
}

static void VFilter(SmoothParams* const p) {
  const uint8_t* src = p->src_;
  const int w = p->width_;
  uint16_t* const cur = p->cur_;
  const uint16_t* const top = p->top_;
  uint16_t* const out = p->end_;
  uint16_t sum = 0;               
  int x;

  for (x = 0; x < w; ++x) {
    uint16_t new_value;
    sum += src[x];
    new_value = top[x] + sum;
    out[x] = new_value - cur[x];  
    cur[x] = new_value;
  }
  
  p->top_ = p->cur_;
  p->cur_ += w;
  if (p->cur_ == p->end_) p->cur_ = p->start_;  
  
  
  if (p->row_ >= 0 && p->row_ < p->height_ - 1) {
    p->src_ += p->width_;
  }
}

static void HFilter(SmoothParams* const p) {
  const uint16_t* const in = p->end_;
  uint16_t* const out = p->average_;
  const uint32_t scale = p->scale_;
  const int w = p->width_;
  const int r = p->radius_;

  int x;
  for (x = 0; x <= r; ++x) {   
    const uint16_t delta = in[x + r - 1] + in[r - x];
    out[x] = (delta * scale) >> FIX;
  }
  for (; x < w - r; ++x) {     
    const uint16_t delta = in[x + r] - in[x - r - 1];
    out[x] = (delta * scale) >> FIX;
  }
  for (; x < w; ++x) {         
    const uint16_t delta =
        2 * in[w - 1] - in[2 * w - 2 - r - x] - in[x - r - 1];
    out[x] = (delta * scale) >> FIX;
  }
}

static void ApplyFilter(SmoothParams* const p) {
  const uint16_t* const average = p->average_;
  const int w = p->width_;
  const int16_t* const correction = p->correction_;
#if defined(USE_DITHERING)
  const uint8_t* const dither = kOrderedDither[p->row_ % DSIZE];
#endif
  uint8_t* const dst = p->dst_;
  int x;
  for (x = 0; x < w; ++x) {
    const int v = dst[x];
    if (v < p->max_ && v > p->min_) {
      const int c = (v << DFIX) + correction[average[x] - (v << LFIX)];
#if defined(USE_DITHERING)
      dst[x] = clip_8b(c + dither[x % DSIZE]);
#else
      dst[x] = clip_8b(c);
#endif
    }
  }
  p->dst_ += w;  
}


static void InitCorrectionLUT(int16_t* const lut, int min_dist) {
  
  
  
  
  
  
  const int threshold1 = min_dist << LFIX;
  const int threshold2 = (3 * threshold1) >> 2;
  const int max_threshold = threshold2 << DFIX;
  const int delta = threshold1 - threshold2;
  int i;
  for (i = 1; i <= LUT_SIZE; ++i) {
    int c = (i <= threshold2) ? (i << DFIX)
          : (i < threshold1) ? max_threshold * (threshold1 - i) / delta
          : 0;
    c >>= LFIX;
    lut[+i] = +c;
    lut[-i] = -c;
  }
  lut[0] = 0;
}

static void CountLevels(const uint8_t* const data, int size,
                        SmoothParams* const p) {
  int i, last_level;
  uint8_t used_levels[256] = { 0 };
  p->min_ = 255;
  p->max_ = 0;
  for (i = 0; i < size; ++i) {
    const int v = data[i];
    if (v < p->min_) p->min_ = v;
    if (v > p->max_) p->max_ = v;
    used_levels[v] = 1;
  }
  
  p->min_level_dist_ = p->max_ - p->min_;
  last_level = -1;
  for (i = 0; i < 256; ++i) {
    if (used_levels[i]) {
      ++p->num_levels_;
      if (last_level >= 0) {
        const int level_dist = i - last_level;
        if (level_dist < p->min_level_dist_) {
          p->min_level_dist_ = level_dist;
        }
      }
      last_level = i;
    }
  }
}

static int InitParams(uint8_t* const data, int width, int height,
                      int radius, SmoothParams* const p) {
  const int R = 2 * radius + 1;  

  const size_t size_scratch_m = (R + 1) * width * sizeof(*p->start_);
  const size_t size_m =  width * sizeof(*p->average_);
  const size_t size_lut = (1 + 2 * LUT_SIZE) * sizeof(*p->correction_);
  const size_t total_size = size_scratch_m + size_m + size_lut;
  uint8_t* mem = (uint8_t*)WebPSafeMalloc(1U, total_size);

  if (mem == NULL) return 0;
  p->mem_ = (void*)mem;

  p->start_ = (uint16_t*)mem;
  p->cur_ = p->start_;
  p->end_ = p->start_ + R * width;
  p->top_ = p->end_ - width;
  memset(p->top_, 0, width * sizeof(*p->top_));
  mem += size_scratch_m;

  p->average_ = (uint16_t*)mem;
  mem += size_m;

  p->width_ = width;
  p->height_ = height;
  p->src_ = data;
  p->dst_ = data;
  p->radius_ = radius;
  p->scale_ = (1 << (FIX + LFIX)) / (R * R);  
  p->row_ = -radius;

  
  CountLevels(data, width * height, p);

  
  p->correction_ = ((int16_t*)mem) + LUT_SIZE;
  InitCorrectionLUT(p->correction_, p->min_level_dist_);

  return 1;
}

static void CleanupParams(SmoothParams* const p) {
  WebPSafeFree(p->mem_);
}

int WebPDequantizeLevels(uint8_t* const data, int width, int height,
                         int strength) {
  const int radius = 4 * strength / 100;
  if (strength < 0 || strength > 100) return 0;
  if (data == NULL || width <= 0 || height <= 0) return 0;  
  if (radius > 0) {
    SmoothParams p;
    memset(&p, 0, sizeof(p));
    if (!InitParams(data, width, height, radius, &p)) return 0;
    if (p.num_levels_ > 2) {
      for (; p.row_ < p.height_; ++p.row_) {
        VFilter(&p);  
        
        
        if (p.row_ >= p.radius_) {
          HFilter(&p);
          ApplyFilter(&p);
        }
      }
    }
    CleanupParams(&p);
  }
  return 1;
}