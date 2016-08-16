// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_APPS_APP_INFO_DIALOG_APP_INFO_HEADER_PANEL_H_
#define CHROME_BROWSER_UI_VIEWS_APPS_APP_INFO_DIALOG_APP_INFO_HEADER_PANEL_H_

#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/views/apps/app_info_dialog/app_info_panel.h"
#include "ui/views/controls/link_listener.h"

class Profile;
namespace extensions {
class Extension;
}

namespace gfx {
class Image;
}

namespace views {
class ImageView;
class Label;
class Link;
class View;
}

class AppInfoHeaderPanel : public AppInfoPanel,
                           public views::LinkListener,
                           public base::SupportsWeakPtr<AppInfoHeaderPanel> {
 public:
  AppInfoHeaderPanel(Profile* profile, const extensions::Extension* app);
  virtual ~AppInfoHeaderPanel();

 private:
  void CreateControls();
  void LayoutControls();

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  
  void LoadAppImageAsync();
  
  void OnAppImageLoaded(const gfx::Image& image);

  
  
  void ShowAppInWebStore() const;
  bool CanShowAppInWebStore() const;

  // Displays the licenses for the app. Must only be called if
  // CanDisplayLicenses() returns true.
  void DisplayLicenses();
  bool CanDisplayLicenses();

  
  views::ImageView* app_icon_;
  views::Label* app_name_label_;
  views::Link* view_in_store_link_;
  views::Link* licenses_link_;

  base::WeakPtrFactory<AppInfoHeaderPanel> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(AppInfoHeaderPanel);
};

#endif  