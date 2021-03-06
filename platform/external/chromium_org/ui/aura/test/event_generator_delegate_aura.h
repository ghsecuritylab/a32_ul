// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_TEST_EVENT_GENERATOR_DELEGATE_AURA_H_
#define UI_AURA_TEST_EVENT_GENERATOR_DELEGATE_AURA_H_

#include "ui/events/test/event_generator.h"

namespace aura {
class Window;
class WindowTreeHost;

namespace client {
class ScreenPositionClient;
}

namespace test {

void InitializeAuraEventGeneratorDelegate();

class EventGeneratorDelegateAura : public ui::test::EventGeneratorDelegate {
 public:
  EventGeneratorDelegateAura();
  virtual ~EventGeneratorDelegateAura();

  
  virtual WindowTreeHost* GetHostAt(const gfx::Point& point) const = 0;

  
  
  
  virtual client::ScreenPositionClient* GetScreenPositionClient(
      const aura::Window* window) const = 0;

  
  virtual ui::EventTarget* GetTargetAt(const gfx::Point& location) OVERRIDE;
  virtual ui::EventSource* GetEventSource(ui::EventTarget* target) OVERRIDE;
  virtual gfx::Point CenterOfTarget(
      const ui::EventTarget* target) const OVERRIDE;
  virtual gfx::Point CenterOfWindow(gfx::NativeWindow window) const OVERRIDE;
  virtual void ConvertPointFromTarget(const ui::EventTarget* target,
                                      gfx::Point* point) const OVERRIDE;
  virtual void ConvertPointToTarget(const ui::EventTarget* target,
                                    gfx::Point* point) const OVERRIDE;
  virtual void ConvertPointFromHost(const ui::EventTarget* hosted_target,
                                    gfx::Point* point) const OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(EventGeneratorDelegateAura);
};

}  
}  

#endif  
