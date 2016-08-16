// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DBUS_STRING_UTIL_H_
#define DBUS_STRING_UTIL_H_

#include <string>

#include "dbus/dbus_export.h"

namespace dbus {

CHROME_DBUS_EXPORT bool IsValidObjectPath(const std::string& value);

}  

#endif  