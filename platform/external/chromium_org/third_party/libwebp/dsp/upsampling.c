// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#include "./dsp.h"
#include "./yuv.h"

#include <assert.h>


#ifdef FANCY_UPSAMPLING

WebPUpsampleLinePairFunc WebPUpsamplers[MODE_LAST];


#define LOAD_UV(u, v) ((u) | ((v) << 16))

#define UPSAMPLE_FUNC(FUNC_NAME, FUNC, XSTEP)                                  \
static void FUNC_NAME(const uint8_t* top_y, const uint8_t* bottom_y,           \
                      const uint8_t* top_u, const uint8_t* top_v,              \
                      const uint8_t* cur_u, const uint8_t* cur_v,              \
                      uint8_t* top_dst, uint8_t* bottom_dst, int len) {        \
  int x;                                                                       \
  const int last_pixel_pair = (len - 1) >> 1;                                  \
  uint32_t tl_uv = LOAD_UV(top_u[0], top_v[0]);           \
  uint32_t l_uv  = LOAD_UV(cur_u[0], cur_v[0]);               \
  assert(top_y != NULL);                                                       \
  {                                                                            \
    const uint32_t uv0 = (3 * tl_uv + l_uv + 0x00020002u) >> 2;                \
    FUNC(top_y[0], uv0 & 0xff, (uv0 >> 16), top_dst);                          \
  }                                                                            \
  if (bottom_y != NULL) {                                                      \
    const uint32_t uv0 = (3 * l_uv + tl_uv + 0x00020002u) >> 2;                \
    FUNC(bottom_y[0], uv0 & 0xff, (uv0 >> 16), bottom_dst);                    \
  }                                                                            \
  for (x = 1; x <= last_pixel_pair; ++x) {                                     \
    const uint32_t t_uv = LOAD_UV(top_u[x], top_v[x]);         \
    const uint32_t uv   = LOAD_UV(cur_u[x], cur_v[x]);             \
    \
    const uint32_t avg = tl_uv + t_uv + l_uv + uv + 0x00080008u;               \
    const uint32_t diag_12 = (avg + 2 * (t_uv + l_uv)) >> 3;                   \
    const uint32_t diag_03 = (avg + 2 * (tl_uv + uv)) >> 3;                    \
    {                                                                          \
      const uint32_t uv0 = (diag_12 + tl_uv) >> 1;                             \
      const uint32_t uv1 = (diag_03 + t_uv) >> 1;                              \
      FUNC(top_y[2 * x - 1], uv0 & 0xff, (uv0 >> 16),                          \
           top_dst + (2 * x - 1) * XSTEP);                                     \
      FUNC(top_y[2 * x - 0], uv1 & 0xff, (uv1 >> 16),                          \
           top_dst + (2 * x - 0) * XSTEP);                                     \
    }                                                                          \
    if (bottom_y != NULL) {                                                    \
      const uint32_t uv0 = (diag_03 + l_uv) >> 1;                              \
      const uint32_t uv1 = (diag_12 + uv) >> 1;                                \
      FUNC(bottom_y[2 * x - 1], uv0 & 0xff, (uv0 >> 16),                       \
           bottom_dst + (2 * x - 1) * XSTEP);                                  \
      FUNC(bottom_y[2 * x + 0], uv1 & 0xff, (uv1 >> 16),                       \
           bottom_dst + (2 * x + 0) * XSTEP);                                  \
    }                                                                          \
    tl_uv = t_uv;                                                              \
    l_uv = uv;                                                                 \
  }                                                                            \
  if (!(len & 1)) {                                                            \
    {                                                                          \
      const uint32_t uv0 = (3 * tl_uv + l_uv + 0x00020002u) >> 2;              \
      FUNC(top_y[len - 1], uv0 & 0xff, (uv0 >> 16),                            \
           top_dst + (len - 1) * XSTEP);                                       \
    }                                                                          \
    if (bottom_y != NULL) {                                                    \
      const uint32_t uv0 = (3 * l_uv + tl_uv + 0x00020002u) >> 2;              \
      FUNC(bottom_y[len - 1], uv0 & 0xff, (uv0 >> 16),                         \
           bottom_dst + (len - 1) * XSTEP);                                    \
    }                                                                          \
  }                                                                            \
}

UPSAMPLE_FUNC(UpsampleRgbLinePair,  VP8YuvToRgb,  3)
UPSAMPLE_FUNC(UpsampleBgrLinePair,  VP8YuvToBgr,  3)
UPSAMPLE_FUNC(UpsampleRgbaLinePair, VP8YuvToRgba, 4)
UPSAMPLE_FUNC(UpsampleBgraLinePair, VP8YuvToBgra, 4)
UPSAMPLE_FUNC(UpsampleArgbLinePair, VP8YuvToArgb, 4)
UPSAMPLE_FUNC(UpsampleRgba4444LinePair, VP8YuvToRgba4444, 2)
UPSAMPLE_FUNC(UpsampleRgb565LinePair,  VP8YuvToRgb565,  2)

#undef LOAD_UV
#undef UPSAMPLE_FUNC

#endif  


#if !defined(FANCY_UPSAMPLING)
#define DUAL_SAMPLE_FUNC(FUNC_NAME, FUNC)                                      \
static void FUNC_NAME(const uint8_t* top_y, const uint8_t* bot_y,              \
                      const uint8_t* top_u, const uint8_t* top_v,              \
                      const uint8_t* bot_u, const uint8_t* bot_v,              \
                      uint8_t* top_dst, uint8_t* bot_dst, int len) {           \
  const int half_len = len >> 1;                                               \
  int x;                                                                       \
  assert(top_dst != NULL);                                                     \
  {                                                                            \
    for (x = 0; x < half_len; ++x) {                                           \
      FUNC(top_y[2 * x + 0], top_u[x], top_v[x], top_dst + 8 * x + 0);         \
      FUNC(top_y[2 * x + 1], top_u[x], top_v[x], top_dst + 8 * x + 4);         \
    }                                                                          \
    if (len & 1) FUNC(top_y[2 * x + 0], top_u[x], top_v[x], top_dst + 8 * x);  \
  }                                                                            \
  if (bot_dst != NULL) {                                                       \
    for (x = 0; x < half_len; ++x) {                                           \
      FUNC(bot_y[2 * x + 0], bot_u[x], bot_v[x], bot_dst + 8 * x + 0);         \
      FUNC(bot_y[2 * x + 1], bot_u[x], bot_v[x], bot_dst + 8 * x + 4);         \
    }                                                                          \
    if (len & 1) FUNC(bot_y[2 * x + 0], bot_u[x], bot_v[x], bot_dst + 8 * x);  \
  }                                                                            \
}

DUAL_SAMPLE_FUNC(DualLineSamplerBGRA, VP8YuvToBgra)
DUAL_SAMPLE_FUNC(DualLineSamplerARGB, VP8YuvToArgb)
#undef DUAL_SAMPLE_FUNC

#endif  

WebPUpsampleLinePairFunc WebPGetLinePairConverter(int alpha_is_last) {
  WebPInitUpsamplers();
  VP8YUVInit();
#ifdef FANCY_UPSAMPLING
  return WebPUpsamplers[alpha_is_last ? MODE_BGRA : MODE_ARGB];
#else
  return (alpha_is_last ? DualLineSamplerBGRA : DualLineSamplerARGB);
#endif
}


#define YUV444_FUNC(FUNC_NAME, FUNC, XSTEP)                                    \
static void FUNC_NAME(const uint8_t* y, const uint8_t* u, const uint8_t* v,    \
                      uint8_t* dst, int len) {                                 \
  int i;                                                                       \
  for (i = 0; i < len; ++i) FUNC(y[i], u[i], v[i], &dst[i * XSTEP]);           \
}

YUV444_FUNC(Yuv444ToRgb,      VP8YuvToRgb,  3)
YUV444_FUNC(Yuv444ToBgr,      VP8YuvToBgr,  3)
YUV444_FUNC(Yuv444ToRgba,     VP8YuvToRgba, 4)
YUV444_FUNC(Yuv444ToBgra,     VP8YuvToBgra, 4)
YUV444_FUNC(Yuv444ToArgb,     VP8YuvToArgb, 4)
YUV444_FUNC(Yuv444ToRgba4444, VP8YuvToRgba4444, 2)
YUV444_FUNC(Yuv444ToRgb565,   VP8YuvToRgb565, 2)

#undef YUV444_FUNC

const WebPYUV444Converter WebPYUV444Converters[MODE_LAST] = {
  Yuv444ToRgb,       
  Yuv444ToRgba,      
  Yuv444ToBgr,       
  Yuv444ToBgra,      
  Yuv444ToArgb,      
  Yuv444ToRgba4444,  
  Yuv444ToRgb565,    
  Yuv444ToRgba,      
  Yuv444ToBgra,      
  Yuv444ToArgb,      
  Yuv444ToRgba4444   
};


extern void WebPInitUpsamplersSSE2(void);
extern void WebPInitUpsamplersNEON(void);

void WebPInitUpsamplers(void) {
#ifdef FANCY_UPSAMPLING
  WebPUpsamplers[MODE_RGB]       = UpsampleRgbLinePair;
  WebPUpsamplers[MODE_RGBA]      = UpsampleRgbaLinePair;
  WebPUpsamplers[MODE_BGR]       = UpsampleBgrLinePair;
  WebPUpsamplers[MODE_BGRA]      = UpsampleBgraLinePair;
  WebPUpsamplers[MODE_ARGB]      = UpsampleArgbLinePair;
  WebPUpsamplers[MODE_RGBA_4444] = UpsampleRgba4444LinePair;
  WebPUpsamplers[MODE_RGB_565]   = UpsampleRgb565LinePair;
  WebPUpsamplers[MODE_rgbA]      = UpsampleRgbaLinePair;
  WebPUpsamplers[MODE_bgrA]      = UpsampleBgraLinePair;
  WebPUpsamplers[MODE_Argb]      = UpsampleArgbLinePair;
  WebPUpsamplers[MODE_rgbA_4444] = UpsampleRgba4444LinePair;

  
  if (VP8GetCPUInfo != NULL) {
#if defined(WEBP_USE_SSE2)
    if (VP8GetCPUInfo(kSSE2)) {
      WebPInitUpsamplersSSE2();
    }
#endif
#if defined(WEBP_USE_NEON)
    if (VP8GetCPUInfo(kNEON)) {
      WebPInitUpsamplersNEON();
    }
#endif
  }
#endif  
}
