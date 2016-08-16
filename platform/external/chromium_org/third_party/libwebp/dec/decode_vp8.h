// Copyright 2010 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_WEBP_DECODE_VP8_H_
#define WEBP_WEBP_DECODE_VP8_H_

#include "../webp/decode.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct VP8Io VP8Io;
typedef int (*VP8IoPutHook)(const VP8Io* io);
typedef int (*VP8IoSetupHook)(VP8Io* io);
typedef void (*VP8IoTeardownHook)(const VP8Io* io);

struct VP8Io {
  
  int width, height;         
                             
                             
                             

  
  int mb_y;                  
  int mb_w;                  
  int mb_h;                  
  const uint8_t* y, *u, *v;  
  int y_stride;              
  int uv_stride;             

  void* opaque;              

  
  
  
  
  
  VP8IoPutHook put;

  
  
  
  
  VP8IoSetupHook setup;

  
  
  VP8IoTeardownHook teardown;

  
  // is set when calling setup() hook and can be overwritten by it. It then
  
  int fancy_upsampling;

  
  size_t data_size;
  const uint8_t* data;

  
  
  
  
  int bypass_filtering;

  
  int use_cropping;
  int crop_left, crop_right, crop_top, crop_bottom;

  
  int use_scaling;
  int scaled_width, scaled_height;

  
  
  
  const uint8_t* a;
};

int VP8InitIoInternal(VP8Io* const, int);

int WebPISetIOHooks(WebPIDecoder* const idec,
                    VP8IoPutHook put,
                    VP8IoSetupHook setup,
                    VP8IoTeardownHook teardown,
                    void* user_data);

typedef struct VP8Decoder VP8Decoder;

VP8Decoder* VP8New(void);

static WEBP_INLINE int VP8InitIo(VP8Io* const io) {
  return VP8InitIoInternal(io, WEBP_DECODER_ABI_VERSION);
}

int VP8GetHeaders(VP8Decoder* const dec, VP8Io* const io);

int VP8Decode(VP8Decoder* const dec, VP8Io* const io);

VP8StatusCode VP8Status(VP8Decoder* const dec);

const char* VP8StatusMessage(VP8Decoder* const dec);

void VP8Clear(VP8Decoder* const dec);

void VP8Delete(VP8Decoder* const dec);


WEBP_EXTERN(int) VP8CheckSignature(const uint8_t* const data, size_t data_size);

WEBP_EXTERN(int) VP8GetInfo(
    const uint8_t* data,
    size_t data_size,    
    size_t chunk_size,   
    int* const width, int* const height);

WEBP_EXTERN(int) VP8LCheckSignature(const uint8_t* const data, size_t size);

WEBP_EXTERN(int) VP8LGetInfo(
    const uint8_t* data, size_t data_size,  
    int* const width, int* const height, int* const has_alpha);

#ifdef __cplusplus
}    
#endif

#endif  