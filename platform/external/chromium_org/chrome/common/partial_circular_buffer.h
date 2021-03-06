// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_PARTIAL_CIRCULAR_BUFFER_H_
#define CHROME_COMMON_PARTIAL_CIRCULAR_BUFFER_H_

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"

// The non-wrapping part is never overwritten. The wrapping part will be
// - Length written to the buffer (not including header).
class PartialCircularBuffer {
 public:
  
  
  PartialCircularBuffer(void* buffer, uint32 buffer_size);

  
  
  
  PartialCircularBuffer(void* buffer,
                        uint32 buffer_size,
                        uint32 wrap_position,
                        bool append);

  uint32 Read(void* buffer, uint32 buffer_size);
  void Write(const void* buffer, uint32 buffer_size);

 private:
  friend class PartialCircularBufferTest;

#pragma pack(push)
#pragma pack(4)
  struct BufferData {
    uint32 total_written;
    uint32 wrap_position;
    uint32 end_position;
    uint8 data[1];
  };
#pragma pack(pop)

  void DoWrite(void* dest, const void* src, uint32 num);

  
  BufferData* buffer_data_;
  uint32 memory_buffer_size_;
  uint32 data_size_;
  uint32 position_;

  
  uint32 total_read_;
};

#endif  
