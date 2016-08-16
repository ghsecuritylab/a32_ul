// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_KEYBOARD_KEYBOARD_H_
#define UI_KEYBOARD_KEYBOARD_H_

#include "ui/keyboard/keyboard_export.h"

namespace keyboard {

KEYBOARD_EXPORT void InitializeKeyboard();
KEYBOARD_EXPORT void InitializeWebUIBindings();

KEYBOARD_EXPORT void ResetKeyboardForTesting();

}  

#endif  