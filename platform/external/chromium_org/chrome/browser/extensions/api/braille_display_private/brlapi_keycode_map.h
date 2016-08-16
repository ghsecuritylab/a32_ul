// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_BRAILLE_DISPLAY_PRIVATE_BRLAPI_KEYCODE_MAP_H_
#define CHROME_BROWSER_EXTENSIONS_API_BRAILLE_DISPLAY_PRIVATE_BRLAPI_KEYCODE_MAP_H_

#include "base/memory/scoped_ptr.h"
#include "chrome/common/extensions/api/braille_display_private.h"
#include "library_loaders/libbrlapi.h"

namespace extensions {
namespace api {
namespace braille_display_private {

scoped_ptr<KeyEvent> BrlapiKeyCodeToEvent(brlapi_keyCode_t code);

}  
}  
}  

#endif  