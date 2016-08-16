// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_CORE_APP_LAUNCHER_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_CORE_APP_LAUNCHER_HANDLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/values.h"
#include "chrome/common/extensions/extension_constants.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "extensions/common/extension.h"

namespace extensions {
class Extension;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class Profile;

class CoreAppLauncherHandler : public content::WebUIMessageHandler {
 public:
  CoreAppLauncherHandler();
  virtual ~CoreAppLauncherHandler();

  
  static void RecordAppLaunchType(extension_misc::AppLaunchBucket bucket,
                                  extensions::Manifest::Type app_type);

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  static void RecordAppListSearchLaunch(const extensions::Extension* extension);

  
  static void RecordAppListMainLaunch(const extensions::Extension* extension);

  
  static void RecordWebStoreLaunch();

 private:
  
  
  
  void HandleRecordAppLaunchByUrl(const base::ListValue* args);

  
  
  void RecordAppLaunchByUrl(Profile* profile,
                            std::string escaped_url,
                            extension_misc::AppLaunchBucket bucket);

  
  virtual void RegisterMessages() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(CoreAppLauncherHandler);
};

#endif  