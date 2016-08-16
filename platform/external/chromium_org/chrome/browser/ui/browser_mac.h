// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BROWSER_MAC_H_
#define CHROME_BROWSER_UI_BROWSER_MAC_H_

#include "chrome/browser/signin/signin_promo.h"
#include "chrome/browser/ui/chrome_pages.h"

class Profile;

namespace chrome {

void OpenAboutWindow(Profile* profile);
void OpenHistoryWindow(Profile* profile);
void OpenDownloadsWindow(Profile* profile);
void OpenHelpWindow(Profile* profile, HelpSource source);
void OpenOptionsWindow(Profile* profile);
void OpenSyncSetupWindow(Profile* profile, signin::Source source);
void OpenClearBrowsingDataDialogWindow(Profile* profile);
void OpenImportSettingsDialogWindow(Profile* profile);
void OpenBookmarkManagerWindow(Profile* profile);
void OpenExtensionsWindow(Profile* profile);

}  

#endif  