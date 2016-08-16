// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_GESTURES_SHELF_GESTURE_HANDLER_H_
#define ASH_WM_GESTURES_SHELF_GESTURE_HANDLER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace ui {
class GestureEvent;
}

namespace ash {
class TrayGestureHandler;

class ShelfGestureHandler {
 public:
  ShelfGestureHandler();
  virtual ~ShelfGestureHandler();

  
  
  
  bool ProcessGestureEvent(const ui::GestureEvent& event);

 private:
  bool drag_in_progress_;

  scoped_ptr<TrayGestureHandler> tray_handler_;

  DISALLOW_COPY_AND_ASSIGN(ShelfGestureHandler);
};

}  

#endif  