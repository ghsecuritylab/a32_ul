// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_ENROLLMENT_AUTO_ENROLLMENT_CHECK_SCREEN_ACTOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_ENROLLMENT_AUTO_ENROLLMENT_CHECK_SCREEN_ACTOR_H_

namespace chromeos {

class AutoEnrollmentCheckScreenActor {
 public:
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void OnExit() = 0;

    
    
    virtual void OnActorDestroyed(AutoEnrollmentCheckScreenActor* actor) = 0;
  };

  virtual ~AutoEnrollmentCheckScreenActor() {}

  virtual void Show() = 0;
  virtual void SetDelegate(Delegate* delegate) = 0;
};

}  

#endif  
