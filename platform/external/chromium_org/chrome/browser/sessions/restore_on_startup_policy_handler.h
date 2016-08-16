// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_RESTORE_ON_STARTUP_POLICY_HANDLER_H_
#define CHROME_BROWSER_SESSIONS_RESTORE_ON_STARTUP_POLICY_HANDLER_H_

#include "components/policy/core/browser/configuration_policy_handler.h"

class PrefValueMap;

namespace policy {

class PolicyErrorMap;
class PolicyMap;

class RestoreOnStartupPolicyHandler : public TypeCheckingPolicyHandler {
 public:
  RestoreOnStartupPolicyHandler();
  virtual ~RestoreOnStartupPolicyHandler();

  
  virtual bool CheckPolicySettings(const PolicyMap& policies,
                                   PolicyErrorMap* errors) OVERRIDE;
  virtual void ApplyPolicySettings(const PolicyMap& policies,
                                   PrefValueMap* prefs) OVERRIDE;

 private:
  void ApplyPolicySettingsFromHomePage(const PolicyMap& policies,
                                       PrefValueMap* prefs);

  DISALLOW_COPY_AND_ASSIGN(RestoreOnStartupPolicyHandler);
};

}  

#endif  