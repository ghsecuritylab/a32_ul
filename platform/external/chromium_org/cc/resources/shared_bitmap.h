// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_RESOURCES_SHARED_BITMAP_H_
#define CC_RESOURCES_SHARED_BITMAP_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/shared_memory.h"
#include "cc/base/cc_export.h"
#include "gpu/command_buffer/common/mailbox.h"
#include "ui/gfx/size.h"

namespace base { class SharedMemory; }

namespace cc {
typedef gpu::Mailbox SharedBitmapId;

class CC_EXPORT SharedBitmap {
 public:
  SharedBitmap(base::SharedMemory* memory,
               const SharedBitmapId& id,
               const base::Callback<void(SharedBitmap* bitmap)>& free_callback);

  SharedBitmap(uint8* pixels,
               const SharedBitmapId& id,
               const base::Callback<void(SharedBitmap* bitmap)>& free_callback);

  ~SharedBitmap();

  bool operator<(const SharedBitmap& right) const {
    if (memory_ < right.memory_)
      return true;
    if (memory_ > right.memory_)
      return false;
    return id_ < right.id_;
  }

  uint8* pixels() { return pixels_; }

  base::SharedMemory* memory() { return memory_; }

  SharedBitmapId id() { return id_; }

  
  static bool SizeInBytes(const gfx::Size& size, size_t* size_in_bytes);
  
  
  static size_t CheckedSizeInBytes(const gfx::Size& size);
  
  
  static size_t UncheckedSizeInBytes(const gfx::Size& size);
  
  static bool VerifySizeInBytes(const gfx::Size& size);

  static SharedBitmapId GenerateId();

 private:
  base::SharedMemory* memory_;
  uint8* pixels_;
  SharedBitmapId id_;
  base::Callback<void(SharedBitmap* bitmap)> free_callback_;

  DISALLOW_COPY_AND_ASSIGN(SharedBitmap);
};

}  

#endif  