// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_VIEWS_DELEGATE_H_
#define UI_VIEWS_VIEWS_DELEGATE_H_

#include <string>

#if defined(OS_WIN)
#include <windows.h>
#endif

#include "base/strings/string16.h"
#include "ui/accessibility/ax_enums.h"
#include "ui/base/ui_base_types.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"
#include "ui/views/widget/widget.h"

namespace base {
class TimeDelta;
}

namespace content {
class WebContents;
class BrowserContext;
class SiteInstance;
}

namespace gfx {
class ImageSkia;
class Rect;
}

namespace ui {
class ContextFactory;
}

namespace views {

class NativeWidget;
class NonClientFrameView;
class ViewsTouchSelectionControllerFactory;
class View;
class Widget;
namespace internal {
class NativeWidgetDelegate;
}

class VIEWS_EXPORT ViewsDelegate {
 public:
#if defined(OS_WIN)
  enum AppbarAutohideEdge {
    EDGE_TOP    = 1 << 0,
    EDGE_LEFT   = 1 << 1,
    EDGE_BOTTOM = 1 << 2,
    EDGE_RIGHT  = 1 << 3,
  };
#endif

  ViewsDelegate();
  virtual ~ViewsDelegate();

  
  
  virtual void SaveWindowPlacement(const Widget* widget,
                                   const std::string& window_name,
                                   const gfx::Rect& bounds,
                                   ui::WindowShowState show_state);

  
  
  virtual bool GetSavedWindowPlacement(const Widget* widget,
                                       const std::string& window_name,
                                       gfx::Rect* bounds,
                                       ui::WindowShowState* show_state) const;

  virtual void NotifyAccessibilityEvent(View* view, ui::AXEvent event_type);

  
  
  virtual void NotifyMenuItemFocused(const base::string16& menu_name,
                                     const base::string16& menu_item_name,
                                     int item_index,
                                     int item_count,
                                     bool has_submenu);

#if defined(OS_WIN)
  
  virtual HICON GetDefaultWindowIcon() const;
  
  
  virtual bool IsWindowInMetro(gfx::NativeWindow window) const;
#elif defined(OS_LINUX) && !defined(OS_CHROMEOS)
  virtual gfx::ImageSkia* GetDefaultWindowIcon() const;
#endif

  
  
  
  virtual NonClientFrameView* CreateDefaultNonClientFrameView(Widget* widget);

  
  
  virtual void AddRef();
  virtual void ReleaseRef();

  
  virtual content::WebContents* CreateWebContents(
      content::BrowserContext* browser_context,
      content::SiteInstance* site_instance);

  
  virtual void OnBeforeWidgetInit(Widget::InitParams* params,
                                  internal::NativeWidgetDelegate* delegate) = 0;

  
  virtual base::TimeDelta GetDefaultTextfieldObscuredRevealDuration();

  
  
  
  
  virtual bool WindowManagerProvidesTitleBar(bool maximized);

#if defined(USE_AURA)
  
  virtual ui::ContextFactory* GetContextFactory();
#endif

#if defined(OS_WIN)
  
  
  
  
  
  
  virtual int GetAppbarAutohideEdges(HMONITOR monitor,
                                     const base::Closure& callback);
#endif

  
  static ViewsDelegate* views_delegate;

 private:
  scoped_ptr<ViewsTouchSelectionControllerFactory> views_tsc_factory_;

  DISALLOW_COPY_AND_ASSIGN(ViewsDelegate);
};

}  

#endif  