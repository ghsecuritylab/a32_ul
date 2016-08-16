// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#include <stdlib.h>
#include "./alphai.h"
#include "./vp8i.h"
#include "./vp8li.h"
#include "../utils/quant_levels_dec.h"
#include "../utils/utils.h"
#include "../webp/format_constants.h"


ALPHDecoder* ALPHNew(void) {
  ALPHDecoder* const dec = (ALPHDecoder*)WebPSafeCalloc(1ULL, sizeof(*dec));
  return dec;
}

void ALPHDelete(ALPHDecoder* const dec) {
  if (dec != NULL) {
    VP8LDelete(dec->vp8l_dec_);
    dec->vp8l_dec_ = NULL;
    WebPSafeFree(dec);
  }
}


static int ALPHInit(ALPHDecoder* const dec, const uint8_t* data,
                    size_t data_size, int width, int height, uint8_t* output) {
  int ok = 0;
  const uint8_t* const alpha_data = data + ALPHA_HEADER_LEN;
  const size_t alpha_data_size = data_size - ALPHA_HEADER_LEN;
  int rsrv;

  assert(width > 0 && height > 0);
  assert(data != NULL && output != NULL);

  dec->width_ = width;
  dec->height_ = height;

  if (data_size <= ALPHA_HEADER_LEN) {
    return 0;
  }

  dec->method_ = (data[0] >> 0) & 0x03;
  dec->filter_ = (data[0] >> 2) & 0x03;
  dec->pre_processing_ = (data[0] >> 4) & 0x03;
  rsrv = (data[0] >> 6) & 0x03;
  if (dec->method_ < ALPHA_NO_COMPRESSION ||
      dec->method_ > ALPHA_LOSSLESS_COMPRESSION ||
      dec->filter_ >= WEBP_FILTER_LAST ||
      dec->pre_processing_ > ALPHA_PREPROCESSED_LEVELS ||
      rsrv != 0) {
    return 0;
  }

  if (dec->method_ == ALPHA_NO_COMPRESSION) {
    const size_t alpha_decoded_size = dec->width_ * dec->height_;
    ok = (alpha_data_size >= alpha_decoded_size);
  } else {
    assert(dec->method_ == ALPHA_LOSSLESS_COMPRESSION);
    ok = VP8LDecodeAlphaHeader(dec, alpha_data, alpha_data_size, output);
  }
  return ok;
}

static int ALPHDecode(VP8Decoder* const dec, int row, int num_rows) {
  ALPHDecoder* const alph_dec = dec->alph_dec_;
  const int width = alph_dec->width_;
  const int height = alph_dec->height_;
  WebPUnfilterFunc unfilter_func = WebPUnfilters[alph_dec->filter_];
  uint8_t* const output = dec->alpha_plane_;
  if (alph_dec->method_ == ALPHA_NO_COMPRESSION) {
    const size_t offset = row * width;
    const size_t num_pixels = num_rows * width;
    assert(dec->alpha_data_size_ >= ALPHA_HEADER_LEN + offset + num_pixels);
    memcpy(dec->alpha_plane_ + offset,
           dec->alpha_data_ + ALPHA_HEADER_LEN + offset, num_pixels);
  } else {  
    assert(alph_dec->vp8l_dec_ != NULL);
    if (!VP8LDecodeAlphaImageStream(alph_dec, row + num_rows)) {
      return 0;
    }
  }

  if (unfilter_func != NULL) {
    unfilter_func(width, height, width, row, num_rows, output);
  }

  if (row + num_rows == dec->pic_hdr_.height_) {
    dec->is_alpha_decoded_ = 1;
  }
  return 1;
}


const uint8_t* VP8DecompressAlphaRows(VP8Decoder* const dec,
                                      int row, int num_rows) {
  const int width = dec->pic_hdr_.width_;
  const int height = dec->pic_hdr_.height_;

  if (row < 0 || num_rows <= 0 || row + num_rows > height) {
    return NULL;    
  }

  if (row == 0) {
    
    assert(dec->alpha_plane_ != NULL);
    dec->alph_dec_ = ALPHNew();
    if (dec->alph_dec_ == NULL) return NULL;
    if (!ALPHInit(dec->alph_dec_, dec->alpha_data_, dec->alpha_data_size_,
                  width, height, dec->alpha_plane_)) {
      ALPHDelete(dec->alph_dec_);
      dec->alph_dec_ = NULL;
      return NULL;
    }
    
    if (dec->alph_dec_->pre_processing_ != ALPHA_PREPROCESSED_LEVELS) {
      dec->alpha_dithering_ = 0;  
    } else {
      num_rows = height;          
    }
  }

  if (!dec->is_alpha_decoded_) {
    int ok = 0;
    assert(dec->alph_dec_ != NULL);
    ok = ALPHDecode(dec, row, num_rows);
    if (ok && dec->alpha_dithering_ > 0) {
      ok = WebPDequantizeLevels(dec->alpha_plane_, width, height,
                                dec->alpha_dithering_);
    }
    if (!ok || dec->is_alpha_decoded_) {
      ALPHDelete(dec->alph_dec_);
      dec->alph_dec_ = NULL;
    }
    if (!ok) return NULL;  
  }

  
  return dec->alpha_plane_ + row * width;
}