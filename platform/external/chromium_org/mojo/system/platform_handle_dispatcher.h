// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_PLATFORM_HANDLE_DISPATCHER_H_
#define MOJO_SYSTEM_PLATFORM_HANDLE_DISPATCHER_H_

#include "base/macros.h"
#include "mojo/embedder/scoped_platform_handle.h"
#include "mojo/system/simple_dispatcher.h"
#include "mojo/system/system_impl_export.h"

namespace mojo {
namespace system {

class MOJO_SYSTEM_IMPL_EXPORT PlatformHandleDispatcher
    : public SimpleDispatcher {
 public:
  explicit PlatformHandleDispatcher(
      embedder::ScopedPlatformHandle platform_handle);

  embedder::ScopedPlatformHandle PassPlatformHandle();

  
  virtual Type GetType() const OVERRIDE;

  
  
  static scoped_refptr<PlatformHandleDispatcher> Deserialize(
      Channel* channel,
      const void* source,
      size_t size,
      embedder::PlatformHandleVector* platform_handles);

 private:
  virtual ~PlatformHandleDispatcher();

  
  virtual void CloseImplNoLock() OVERRIDE;
  virtual scoped_refptr<Dispatcher>
      CreateEquivalentDispatcherAndCloseImplNoLock() OVERRIDE;
  virtual void StartSerializeImplNoLock(Channel* channel,
                                        size_t* max_size,
                                        size_t* max_platform_handles) OVERRIDE;
  virtual bool EndSerializeAndCloseImplNoLock(
      Channel* channel,
      void* destination,
      size_t* actual_size,
      embedder::PlatformHandleVector* platform_handles) OVERRIDE;

  embedder::ScopedPlatformHandle platform_handle_;

  DISALLOW_COPY_AND_ASSIGN(PlatformHandleDispatcher);
};

}  
}  

#endif  
