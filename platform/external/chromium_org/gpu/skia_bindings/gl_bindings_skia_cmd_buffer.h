// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_SKIA_BINDINGS_GL_BINDINGS_SKIA_CMD_BUFFER_H_
#define GPU_SKIA_BINDINGS_GL_BINDINGS_SKIA_CMD_BUFFER_H_

#include "third_party/skia/include/core/SkTypes.h"

struct GrGLInterface;

namespace skia_bindings {

GrGLInterface* CreateCommandBufferSkiaGLBinding();

}  

#endif  