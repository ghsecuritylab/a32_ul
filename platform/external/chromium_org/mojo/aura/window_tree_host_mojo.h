// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_EXAMPLES_AURA_DEMO_WINDOW_TREE_HOST_VIEW_MANAGER_H_
#define MOJO_EXAMPLES_AURA_DEMO_WINDOW_TREE_HOST_VIEW_MANAGER_H_

#include "mojo/services/public/cpp/view_manager/view_observer.h"
#include "ui/aura/window_tree_host.h"
#include "ui/events/event_source.h"
#include "ui/gfx/geometry/rect.h"

class SkBitmap;

namespace ui {
class Compositor;
}

namespace mojo {

class WindowTreeHostMojoDelegate;

class WindowTreeHostMojo : public aura::WindowTreeHost,
                           public ui::EventSource,
                           public ViewObserver {
 public:
  WindowTreeHostMojo(View* view, WindowTreeHostMojoDelegate* delegate);
  virtual ~WindowTreeHostMojo();

  
  static WindowTreeHostMojo* ForCompositor(ui::Compositor* compositor);

  const gfx::Rect& bounds() const { return bounds_; }

  
  
  void SetContents(const SkBitmap& contents);

  ui::EventDispatchDetails SendEventToProcessor(ui::Event* event) {
    return ui::EventSource::SendEventToProcessor(event);
  }

 private:
  
  virtual ui::EventSource* GetEventSource() OVERRIDE;
  virtual gfx::AcceleratedWidget GetAcceleratedWidget() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual gfx::Rect GetBounds() const OVERRIDE;
  virtual void SetBounds(const gfx::Rect& bounds) OVERRIDE;
  virtual gfx::Point GetLocationOnNativeScreen() const OVERRIDE;
  virtual void SetCapture() OVERRIDE;
  virtual void ReleaseCapture() OVERRIDE;
  virtual void PostNativeEvent(const base::NativeEvent& native_event) OVERRIDE;
  virtual void SetCursorNative(gfx::NativeCursor cursor) OVERRIDE;
  virtual void MoveCursorToNative(const gfx::Point& location) OVERRIDE;
  virtual void OnCursorVisibilityChangedNative(bool show) OVERRIDE;

  
  virtual ui::EventProcessor* GetEventProcessor() OVERRIDE;

  
  virtual void OnViewBoundsChanged(
      View* view,
      const gfx::Rect& old_bounds,
      const gfx::Rect& new_bounds) OVERRIDE;

  View* view_;

  gfx::Rect bounds_;

  WindowTreeHostMojoDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(WindowTreeHostMojo);
};

}  

#endif  