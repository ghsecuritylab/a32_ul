// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_BUTTON_BUTTON_H_
#define UI_VIEWS_CONTROLS_BUTTON_BUTTON_H_

#include "ui/native_theme/native_theme.h"
#include "ui/views/view.h"

namespace views {

class Button;
class Event;

class VIEWS_EXPORT ButtonListener {
 public:
  virtual void ButtonPressed(Button* sender, const ui::Event& event) = 0;

 protected:
  virtual ~ButtonListener() {}
};

class VIEWS_EXPORT Button : public View {
 public:
  virtual ~Button();

  
  enum ButtonState {
    STATE_NORMAL = 0,
    STATE_HOVERED,
    STATE_PRESSED,
    STATE_DISABLED,
    STATE_COUNT,
  };

  
  
  enum ButtonStyle {
    STYLE_BUTTON = 0,
    STYLE_TEXTBUTTON,
    STYLE_COUNT,
  };

  static ButtonState GetButtonStateFrom(ui::NativeTheme::State state);

  void SetTooltipText(const base::string16& tooltip_text);

  int tag() const { return tag_; }
  void set_tag(int tag) { tag_ = tag; }

  void SetAccessibleName(const base::string16& name);

  
  virtual bool GetTooltipText(const gfx::Point& p,
                              base::string16* tooltip) const OVERRIDE;
  virtual void GetAccessibleState(ui::AXViewState* state) OVERRIDE;

 protected:
  
  
  
  explicit Button(ButtonListener* listener);

  
  virtual void NotifyClick(const ui::Event& event);

  
  ButtonListener* listener_;

 private:
  
  base::string16 tooltip_text_;

  
  base::string16 accessible_name_;

  
  
  int tag_;

  DISALLOW_COPY_AND_ASSIGN(Button);
};

}  

#endif  