// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROME_BROWSER_FIELD_TRIALS_DESKTOP_H_
#define CHROME_BROWSER_CHROME_BROWSER_FIELD_TRIALS_DESKTOP_H_

#include "base/basictypes.h"
#include "base/time/time.h"

class PrefService;

namespace base {
class CommandLine;
}

namespace chrome {

void SetupDesktopFieldTrials(const base::CommandLine& parsed_command_line,
                             PrefService* local_state);

}  

#endif  
