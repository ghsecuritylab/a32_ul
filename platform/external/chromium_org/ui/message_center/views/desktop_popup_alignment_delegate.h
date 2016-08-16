// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_DESKTOP_POPUP_ALIGNMENT_DELEGATE_H_
#define UI_MESSAGE_CENTER_VIEWS_DESKTOP_POPUP_ALIGNMENT_DELEGATE_H_

#include "base/macros.h"
#include "ui/gfx/display_observer.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/message_center/views/popup_alignment_delegate.h"

namespace gfx {
class Screen;
}

namespace message_center {
namespace test {
class MessagePopupCollectionTest;
}

class MESSAGE_CENTER_EXPORT DesktopPopupAlignmentDelegate
    : public PopupAlignmentDelegate,
      public gfx::DisplayObserver {
 public:
  DesktopPopupAlignmentDelegate();
  virtual ~DesktopPopupAlignmentDelegate();

  void StartObserving(gfx::Screen* screen);

  
  virtual int GetToastOriginX(const gfx::Rect& toast_bounds) const OVERRIDE;
  virtual int GetBaseLine() const OVERRIDE;
  virtual int GetWorkAreaBottom() const OVERRIDE;
  virtual bool IsTopDown() const OVERRIDE;
  virtual bool IsFromLeft() const OVERRIDE;
  virtual void RecomputeAlignment(const gfx::Display& display) OVERRIDE;

 private:
  friend class test::MessagePopupCollectionTest;

  enum PopupAlignment {
    POPUP_ALIGNMENT_TOP = 1 << 0,
    POPUP_ALIGNMENT_LEFT = 1 << 1,
    POPUP_ALIGNMENT_BOTTOM = 1 << 2,
    POPUP_ALIGNMENT_RIGHT = 1 << 3,
  };

  
  virtual void OnDisplayAdded(const gfx::Display& new_display) OVERRIDE;
  virtual void OnDisplayRemoved(const gfx::Display& old_display) OVERRIDE;
  virtual void OnDisplayMetricsChanged(const gfx::Display& display,
                                       uint32_t metrics) OVERRIDE;

  int32_t alignment_;
  int64_t display_id_;
  gfx::Screen* screen_;
  gfx::Rect work_area_;

  DISALLOW_COPY_AND_ASSIGN(DesktopPopupAlignmentDelegate);
};

}  

#endif  