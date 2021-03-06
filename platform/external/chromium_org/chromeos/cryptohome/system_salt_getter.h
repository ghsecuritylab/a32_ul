// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_CRYPTOHOME_SYSTEM_SALT_GETTER_H_
#define CHROMEOS_CRYPTOHOME_SYSTEM_SALT_GETTER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/dbus_method_call_status.h"

namespace chromeos {

class CHROMEOS_EXPORT SystemSaltGetter {
 public:
  typedef base::Callback<void(const std::string& system_salt)>
      GetSystemSaltCallback;

  
  static void Initialize();
  static bool IsInitialized();
  static void Shutdown();
  static SystemSaltGetter* Get();

  
  static std::string ConvertRawSaltToHexString(const std::vector<uint8>& salt);

  
  
  void GetSystemSalt(const GetSystemSaltCallback& callback);

 protected:
  SystemSaltGetter();
  ~SystemSaltGetter();

 private:
  
  void DidWaitForServiceToBeAvailable(const GetSystemSaltCallback& callback,
                                      bool service_is_available);
  void DidGetSystemSalt(const GetSystemSaltCallback& callback,
                        DBusMethodCallStatus call_status,
                        const std::vector<uint8>& system_salt);

  std::string system_salt_;

  base::WeakPtrFactory<SystemSaltGetter> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(SystemSaltGetter);
};

}  

#endif  
