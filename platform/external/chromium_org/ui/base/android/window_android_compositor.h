// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_ANDROID_WINDOW_ANDROID_COMPOSITOR_H_
#define UI_BASE_ANDROID_WINDOW_ANDROID_COMPOSITOR_H_

#include "cc/output/copy_output_request.h"
#include "ui/base/android/system_ui_resource_manager.h"
#include "ui/base/ui_base_export.h"

namespace cc {
class Layer;
}

namespace ui {

class UI_BASE_EXPORT WindowAndroidCompositor {
 public:
  virtual ~WindowAndroidCompositor() {}

  virtual void AttachLayerForReadback(scoped_refptr<cc::Layer> layer) = 0;
  virtual void RequestCopyOfOutputOnRootLayer(
      scoped_ptr<cc::CopyOutputRequest> request) = 0;
  virtual void OnVSync(base::TimeTicks frame_time,
                       base::TimeDelta vsync_period) = 0;
  virtual void SetNeedsAnimate() = 0;
  virtual SystemUIResourceManager& GetSystemUIResourceManager() = 0;
};

}  

#endif  