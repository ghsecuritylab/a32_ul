// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_UTILS_HUFFMAN_ENCODE_H_
#define WEBP_UTILS_HUFFMAN_ENCODE_H_

#include "../webp/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint8_t code;         
  uint8_t extra_bits;   
} HuffmanTreeToken;

typedef struct {
  int       num_symbols;   
  uint8_t*  code_lengths;  
  uint16_t* codes;         
} HuffmanTreeCode;

typedef struct {
  uint32_t total_count_;
  int value_;
  int pool_index_left_;    
  int pool_index_right_;   
} HuffmanTree;

int VP8LCreateCompressedHuffmanTree(const HuffmanTreeCode* const tree,
                                    HuffmanTreeToken* tokens, int max_tokens);

void VP8LCreateHuffmanTree(uint32_t* const histogram, int tree_depth_limit,
                           uint8_t* const buf_rle, HuffmanTree* const huff_tree,
                           HuffmanTreeCode* const tree);

#ifdef __cplusplus
}
#endif

#endif  