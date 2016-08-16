// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_EMBEDDER_PLATFORM_HANDLE_UTILS_H_
#define MOJO_EMBEDDER_PLATFORM_HANDLE_UTILS_H_

#include "mojo/embedder/platform_handle.h"
#include "mojo/embedder/scoped_platform_handle.h"
#include "mojo/system/system_impl_export.h"

namespace mojo {
namespace embedder {

template <typename PlatformHandleContainer>
MOJO_SYSTEM_IMPL_EXPORT inline void CloseAllPlatformHandles(
    PlatformHandleContainer* platform_handles) {
  for (typename PlatformHandleContainer::iterator it =
           platform_handles->begin();
       it != platform_handles->end();
       ++it)
    it->CloseIfNecessary();
}

MOJO_SYSTEM_IMPL_EXPORT ScopedPlatformHandle
    DuplicatePlatformHandle(PlatformHandle platform_handle);

}  
}  

#endif  