// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_PASSWORD_MANAGER_CORE_BROWSER_BROWSER_SAVE_PASSWORD_PROGRESS_LOGGER_H_
#define COMPONENTS_PASSWORD_MANAGER_CORE_BROWSER_BROWSER_SAVE_PASSWORD_PROGRESS_LOGGER_H_

#include <string>

#include "components/autofill/core/common/save_password_progress_logger.h"

namespace password_manager {

class PasswordManagerClient;

class BrowserSavePasswordProgressLogger
    : public autofill::SavePasswordProgressLogger {
 public:
  explicit BrowserSavePasswordProgressLogger(PasswordManagerClient* client);
  virtual ~BrowserSavePasswordProgressLogger();

 protected:
  
  virtual void SendLog(const std::string& log) OVERRIDE;

 private:
  
  
  PasswordManagerClient* const client_;

  DISALLOW_COPY_AND_ASSIGN(BrowserSavePasswordProgressLogger);
};

}  

#endif  
