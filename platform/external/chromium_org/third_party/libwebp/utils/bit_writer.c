// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#include <assert.h>
#include <string.h>   
#include <stdlib.h>

#include "./bit_writer.h"
#include "./endian_inl.h"
#include "./utils.h"


static int BitWriterResize(VP8BitWriter* const bw, size_t extra_size) {
  uint8_t* new_buf;
  size_t new_size;
  const uint64_t needed_size_64b = (uint64_t)bw->pos_ + extra_size;
  const size_t needed_size = (size_t)needed_size_64b;
  if (needed_size_64b != needed_size) {
    bw->error_ = 1;
    return 0;
  }
  if (needed_size <= bw->max_pos_) return 1;
  
  new_size = 2 * bw->max_pos_;
  if (new_size < needed_size) new_size = needed_size;
  if (new_size < 1024) new_size = 1024;
  new_buf = (uint8_t*)WebPSafeMalloc(1ULL, new_size);
  if (new_buf == NULL) {
    bw->error_ = 1;
    return 0;
  }
  if (bw->pos_ > 0) {
    assert(bw->buf_ != NULL);
    memcpy(new_buf, bw->buf_, bw->pos_);
  }
  WebPSafeFree(bw->buf_);
  bw->buf_ = new_buf;
  bw->max_pos_ = new_size;
  return 1;
}

static void kFlush(VP8BitWriter* const bw) {
  const int s = 8 + bw->nb_bits_;
  const int32_t bits = bw->value_ >> s;
  assert(bw->nb_bits_ >= 0);
  bw->value_ -= bits << s;
  bw->nb_bits_ -= 8;
  if ((bits & 0xff) != 0xff) {
    size_t pos = bw->pos_;
    if (!BitWriterResize(bw, bw->run_ + 1)) {
      return;
    }
    if (bits & 0x100) {  
      if (pos > 0) bw->buf_[pos - 1]++;
    }
    if (bw->run_ > 0) {
      const int value = (bits & 0x100) ? 0x00 : 0xff;
      for (; bw->run_ > 0; --bw->run_) bw->buf_[pos++] = value;
    }
    bw->buf_[pos++] = bits;
    bw->pos_ = pos;
  } else {
    bw->run_++;   
  }
}


static const uint8_t kNorm[128] = {  
     7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0
};

static const uint8_t kNewRange[128] = {
  127, 127, 191, 127, 159, 191, 223, 127, 143, 159, 175, 191, 207, 223, 239,
  127, 135, 143, 151, 159, 167, 175, 183, 191, 199, 207, 215, 223, 231, 239,
  247, 127, 131, 135, 139, 143, 147, 151, 155, 159, 163, 167, 171, 175, 179,
  183, 187, 191, 195, 199, 203, 207, 211, 215, 219, 223, 227, 231, 235, 239,
  243, 247, 251, 127, 129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149,
  151, 153, 155, 157, 159, 161, 163, 165, 167, 169, 171, 173, 175, 177, 179,
  181, 183, 185, 187, 189, 191, 193, 195, 197, 199, 201, 203, 205, 207, 209,
  211, 213, 215, 217, 219, 221, 223, 225, 227, 229, 231, 233, 235, 237, 239,
  241, 243, 245, 247, 249, 251, 253, 127
};

int VP8PutBit(VP8BitWriter* const bw, int bit, int prob) {
  const int split = (bw->range_ * prob) >> 8;
  if (bit) {
    bw->value_ += split + 1;
    bw->range_ -= split + 1;
  } else {
    bw->range_ = split;
  }
  if (bw->range_ < 127) {   
    const int shift = kNorm[bw->range_];
    bw->range_ = kNewRange[bw->range_];
    bw->value_ <<= shift;
    bw->nb_bits_ += shift;
    if (bw->nb_bits_ > 0) kFlush(bw);
  }
  return bit;
}

int VP8PutBitUniform(VP8BitWriter* const bw, int bit) {
  const int split = bw->range_ >> 1;
  if (bit) {
    bw->value_ += split + 1;
    bw->range_ -= split + 1;
  } else {
    bw->range_ = split;
  }
  if (bw->range_ < 127) {
    bw->range_ = kNewRange[bw->range_];
    bw->value_ <<= 1;
    bw->nb_bits_ += 1;
    if (bw->nb_bits_ > 0) kFlush(bw);
  }
  return bit;
}

void VP8PutValue(VP8BitWriter* const bw, int value, int nb_bits) {
  int mask;
  for (mask = 1 << (nb_bits - 1); mask; mask >>= 1)
    VP8PutBitUniform(bw, value & mask);
}

void VP8PutSignedValue(VP8BitWriter* const bw, int value, int nb_bits) {
  if (!VP8PutBitUniform(bw, value != 0))
    return;
  if (value < 0) {
    VP8PutValue(bw, ((-value) << 1) | 1, nb_bits + 1);
  } else {
    VP8PutValue(bw, value << 1, nb_bits + 1);
  }
}


int VP8BitWriterInit(VP8BitWriter* const bw, size_t expected_size) {
  bw->range_   = 255 - 1;
  bw->value_   = 0;
  bw->run_     = 0;
  bw->nb_bits_ = -8;
  bw->pos_     = 0;
  bw->max_pos_ = 0;
  bw->error_   = 0;
  bw->buf_     = NULL;
  return (expected_size > 0) ? BitWriterResize(bw, expected_size) : 1;
}

uint8_t* VP8BitWriterFinish(VP8BitWriter* const bw) {
  VP8PutValue(bw, 0, 9 - bw->nb_bits_);
  bw->nb_bits_ = 0;   
  kFlush(bw);
  return bw->buf_;
}

int VP8BitWriterAppend(VP8BitWriter* const bw,
                       const uint8_t* data, size_t size) {
  assert(data != NULL);
  if (bw->nb_bits_ != -8) return 0;   
  if (!BitWriterResize(bw, size)) return 0;
  memcpy(bw->buf_ + bw->pos_, data, size);
  bw->pos_ += size;
  return 1;
}

void VP8BitWriterWipeOut(VP8BitWriter* const bw) {
  if (bw != NULL) {
    WebPSafeFree(bw->buf_);
    memset(bw, 0, sizeof(*bw));
  }
}


#define MIN_EXTRA_SIZE  (32768ULL)

#define VP8L_WRITER_BYTES ((int)sizeof(vp8l_wtype_t))
#define VP8L_WRITER_BITS (VP8L_WRITER_BYTES * 8)
#define VP8L_WRITER_MAX_BITS (8 * (int)sizeof(vp8l_atype_t))

static int VP8LBitWriterResize(VP8LBitWriter* const bw, size_t extra_size) {
  uint8_t* allocated_buf;
  size_t allocated_size;
  const size_t max_bytes = bw->end_ - bw->buf_;
  const size_t current_size = bw->cur_ - bw->buf_;
  const uint64_t size_required_64b = (uint64_t)current_size + extra_size;
  const size_t size_required = (size_t)size_required_64b;
  if (size_required != size_required_64b) {
    bw->error_ = 1;
    return 0;
  }
  if (max_bytes > 0 && size_required <= max_bytes) return 1;
  allocated_size = (3 * max_bytes) >> 1;
  if (allocated_size < size_required) allocated_size = size_required;
  
  allocated_size = (((allocated_size >> 10) + 1) << 10);
  allocated_buf = (uint8_t*)WebPSafeMalloc(1ULL, allocated_size);
  if (allocated_buf == NULL) {
    bw->error_ = 1;
    return 0;
  }
  if (current_size > 0) {
    memcpy(allocated_buf, bw->buf_, current_size);
  }
  WebPSafeFree(bw->buf_);
  bw->buf_ = allocated_buf;
  bw->cur_ = bw->buf_ + current_size;
  bw->end_ = bw->buf_ + allocated_size;
  return 1;
}

int VP8LBitWriterInit(VP8LBitWriter* const bw, size_t expected_size) {
  memset(bw, 0, sizeof(*bw));
  return VP8LBitWriterResize(bw, expected_size);
}

void VP8LBitWriterDestroy(VP8LBitWriter* const bw) {
  if (bw != NULL) {
    WebPSafeFree(bw->buf_);
    memset(bw, 0, sizeof(*bw));
  }
}

void VP8LWriteBits(VP8LBitWriter* const bw, int n_bits, uint32_t bits) {
  assert(n_bits <= 32);
  
  assert(bw->used_ + n_bits <= 2 * VP8L_WRITER_MAX_BITS);
  if (n_bits > 0) {
    
    vp8l_atype_t lbits = bw->bits_;
    int used = bw->used_;
    
    if (VP8L_WRITER_BITS == 16) {
      if (used + n_bits >= VP8L_WRITER_MAX_BITS) {
        
        const int shift = VP8L_WRITER_MAX_BITS - used;
        lbits |= (vp8l_atype_t)bits << used;
        used = VP8L_WRITER_MAX_BITS;
        n_bits -= shift;
        bits >>= shift;
        assert(n_bits <= VP8L_WRITER_MAX_BITS);
      }
    }
    
    while (used >= VP8L_WRITER_BITS) {
      if (bw->cur_ + VP8L_WRITER_BYTES > bw->end_) {
        const uint64_t extra_size = (bw->end_ - bw->buf_) + MIN_EXTRA_SIZE;
        if (extra_size != (size_t)extra_size ||
            !VP8LBitWriterResize(bw, (size_t)extra_size)) {
          bw->cur_ = bw->buf_;
          bw->error_ = 1;
          return;
        }
      }
      *(vp8l_wtype_t*)bw->cur_ = (vp8l_wtype_t)WSWAP((vp8l_wtype_t)lbits);
      bw->cur_ += VP8L_WRITER_BYTES;
      lbits >>= VP8L_WRITER_BITS;
      used -= VP8L_WRITER_BITS;
    }
    
    bw->bits_ = lbits | ((vp8l_atype_t)bits << used);
    bw->used_ = used + n_bits;
  }
}

uint8_t* VP8LBitWriterFinish(VP8LBitWriter* const bw) {
  
  if (VP8LBitWriterResize(bw, (bw->used_ + 7) >> 3)) {
    while (bw->used_ > 0) {
      *bw->cur_++ = (uint8_t)bw->bits_;
      bw->bits_ >>= 8;
      bw->used_ -= 8;
    }
    bw->used_ = 0;
  }
  return bw->buf_;
}
