// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_INFOBARS_CONFIRM_INFOBAR_H_
#define CHROME_BROWSER_UI_VIEWS_INFOBARS_CONFIRM_INFOBAR_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/ui/views/infobars/infobar_view.h"
#include "ui/views/controls/link_listener.h"

class ConfirmInfoBarDelegate;
class ElevationIconSetter;

namespace views {
class Label;
}

class ConfirmInfoBar : public InfoBarView,
                       public views::LinkListener {
 public:
  explicit ConfirmInfoBar(scoped_ptr<ConfirmInfoBarDelegate> delegate);

 private:
  virtual ~ConfirmInfoBar();

  
  virtual void Layout() OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;
  virtual int ContentMinimumWidth() const OVERRIDE;

  
  virtual void LinkClicked(views::Link* source, int event_flags) OVERRIDE;

  ConfirmInfoBarDelegate* GetDelegate();

  
  
  int NonLabelWidth() const;

  views::Label* label_;
  views::LabelButton* ok_button_;
  views::LabelButton* cancel_button_;
  views::Link* link_;
  scoped_ptr<ElevationIconSetter> elevation_icon_setter_;

  DISALLOW_COPY_AND_ASSIGN(ConfirmInfoBar);
};

#endif  