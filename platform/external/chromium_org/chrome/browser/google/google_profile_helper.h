// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_GOOGLE_GOOGLE_PROFILE_HELPER_H__
#define CHROME_BROWSER_GOOGLE_GOOGLE_PROFILE_HELPER_H__

#include <string>

#include "base/basictypes.h"

class GURL;
class Profile;

namespace google_profile_helper {

GURL GetGoogleHomePageURL(Profile* profile);

}  

#endif  
