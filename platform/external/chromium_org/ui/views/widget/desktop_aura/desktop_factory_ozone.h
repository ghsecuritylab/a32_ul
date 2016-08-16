// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_FACTORY_OZONE_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_FACTORY_OZONE_H_

#include "ui/views/views_export.h"

namespace gfx {
class Rect;
class Screen;
}

namespace views {
class DesktopNativeWidgetAura;
class DesktopWindowTreeHost;

namespace internal {
class NativeWidgetDelegate;
}

class VIEWS_EXPORT DesktopFactoryOzone {
 public:
  DesktopFactoryOzone();
  virtual ~DesktopFactoryOzone();

  
  static DesktopFactoryOzone* GetInstance();

  
  static void SetInstance(DesktopFactoryOzone* impl);

  
  
  virtual DesktopWindowTreeHost* CreateWindowTreeHost(
      internal::NativeWidgetDelegate* native_widget_delegate,
      DesktopNativeWidgetAura* desktop_native_widget_aura) = 0;

  
  
  virtual gfx::Screen* CreateDesktopScreen() = 0;

 private:
  static DesktopFactoryOzone* impl_; 
};

}  

#endif 