// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_LOCK_SCREEN_LOCKER_TESTER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_LOCK_SCREEN_LOCKER_TESTER_H_

#include <string>

#include "base/basictypes.h"

namespace views {
class Widget;
}  

namespace chromeos {

class UserContext;

namespace test {

class ScreenLockerTester {
 public:
  ScreenLockerTester();
  virtual ~ScreenLockerTester();

  
  virtual bool IsLocked();

  
  virtual void InjectMockAuthenticator(const UserContext& user_context);

  
  virtual void SetPassword(const std::string& password) = 0;

  
  virtual std::string GetPassword() = 0;

  
  virtual void EnterPassword(const std::string& password) = 0;

  
  virtual void EmulateWindowManagerReady() = 0;

  
  virtual views::Widget* GetWidget() const = 0;

  virtual views::Widget* GetChildWidget() const = 0;
};

}  

}  

#endif  