// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_LOGIN_AUTH_AUTHENTICATOR_H_
#define CHROMEOS_LOGIN_AUTH_AUTHENTICATOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/login/auth/auth_status_consumer.h"
#include "google_apis/gaia/gaia_auth_consumer.h"

class Profile;

namespace chromeos {

class UserContext;

class CHROMEOS_EXPORT Authenticator
    : public base::RefCountedThreadSafe<Authenticator> {
 public:
  explicit Authenticator(AuthStatusConsumer* consumer);

  
  
  virtual void CompleteLogin(Profile* profile,
                             const UserContext& user_context) = 0;

  
  
  
  virtual void AuthenticateToLogin(Profile* profile,
                                   const UserContext& user_context) = 0;

  
  
  
  virtual void AuthenticateToUnlock(const UserContext& user_context) = 0;

  
  virtual void LoginAsSupervisedUser(const UserContext& user_context) = 0;

  
  virtual void LoginRetailMode() = 0;

  
  virtual void LoginOffTheRecord() = 0;

  
  virtual void LoginAsPublicSession(const UserContext& user_context) = 0;

  
  
  
  
  virtual void LoginAsKioskAccount(const std::string& app_user_id,
                                   bool use_guest_mount) = 0;

  
  virtual void OnRetailModeAuthSuccess() = 0;

  
  virtual void OnAuthSuccess() = 0;

  
  virtual void OnAuthFailure(const AuthFailure& error) = 0;

  
  
  
  
  
  
  
  virtual void RecoverEncryptedData(const std::string& old_password) = 0;

  
  
  virtual void ResyncEncryptedData() = 0;

  
  
  Profile* authentication_profile() { return authentication_profile_; }

  
  void SetConsumer(AuthStatusConsumer* consumer);

 protected:
  virtual ~Authenticator();

  AuthStatusConsumer* consumer_;
  Profile* authentication_profile_;

 private:
  friend class base::RefCountedThreadSafe<Authenticator>;

  DISALLOW_COPY_AND_ASSIGN(Authenticator);
};

}  

#endif  