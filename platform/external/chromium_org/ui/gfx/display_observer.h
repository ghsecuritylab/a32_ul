// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_DISPLAY_OBSERVER_H_
#define UI_GFX_DISPLAY_OBSERVER_H_

#include <stdint.h>

#include "ui/gfx/gfx_export.h"

namespace gfx {
class Display;

class GFX_EXPORT DisplayObserver {
 public:
  enum DisplayMetric {
    DISPLAY_METRIC_NONE = 0,
    DISPLAY_METRIC_BOUNDS = 1 << 0,
    DISPLAY_METRIC_WORK_AREA = 1 << 1,
    DISPLAY_METRIC_DEVICE_SCALE_FACTOR = 1 << 2,
    DISPLAY_METRIC_ROTATION = 1 << 3,
  };

  
  virtual void OnDisplayAdded(const Display& new_display) = 0;

  
  virtual void OnDisplayRemoved(const Display& old_display) = 0;

  
  
  virtual void OnDisplayMetricsChanged(const Display& display,
                                       uint32_t changed_metrics) = 0;

 protected:
  virtual ~DisplayObserver();
};

}  

#endif  