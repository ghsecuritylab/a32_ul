// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_TOUCH_TOUCH_OBSERVER_UMA_H_
#define ASH_TOUCH_TOUCH_OBSERVER_UMA_H_

#include <map>

#include "ash/shell.h"
#include "base/memory/singleton.h"
#include "ui/gfx/point.h"
#include "ui/views/widget/widget.h"

namespace aura {
class Window;
}

namespace ash {

class ASH_EXPORT TouchUMA {
 public:
  enum GestureActionType {
    GESTURE_UNKNOWN,
    GESTURE_OMNIBOX_PINCH,
    GESTURE_OMNIBOX_SCROLL,
    GESTURE_TABSTRIP_PINCH,
    GESTURE_TABSTRIP_SCROLL,
    GESTURE_BEZEL_SCROLL,
    GESTURE_DESKTOP_SCROLL,
    GESTURE_DESKTOP_PINCH,
    GESTURE_WEBPAGE_PINCH,
    GESTURE_WEBPAGE_SCROLL,
    GESTURE_WEBPAGE_TAP,
    GESTURE_TABSTRIP_TAP,
    GESTURE_BEZEL_DOWN,
    GESTURE_TABSWITCH_TAP,
    GESTURE_TABNOSWITCH_TAP,
    GESTURE_TABCLOSE_TAP,
    GESTURE_NEWTAB_TAP,
    GESTURE_ROOTVIEWTOP_TAP,
    GESTURE_FRAMEMAXIMIZE_TAP,
    GESTURE_FRAMEVIEW_TAP,
    GESTURE_MAXIMIZE_DOUBLETAP,
    
    
    
    GESTURE_ACTION_COUNT
  };

  
  static TouchUMA* GetInstance();

  void RecordGestureEvent(aura::Window* target,
                          const ui::GestureEvent& event);
  void RecordGestureAction(GestureActionType action);
  void RecordTouchEvent(aura::Window* target,
                        const ui::TouchEvent& event);

 private:
  friend struct DefaultSingletonTraits<TouchUMA>;

  TouchUMA();
  ~TouchUMA();

  void UpdateTouchState(const ui::TouchEvent& event);
  GestureActionType FindGestureActionType(aura::Window* window,
                                          const ui::GestureEvent& event);

  bool is_single_finger_gesture_;
  
  
  bool touch_in_progress_;
  int burst_length_;
  base::TimeDelta last_touch_down_time_;

  DISALLOW_COPY_AND_ASSIGN(TouchUMA);
};

}  

#endif  