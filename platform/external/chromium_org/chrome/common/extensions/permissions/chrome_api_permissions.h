// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_PERMISSIONS_CHROME_API_PERMISSIONS_H_
#define CHROME_COMMON_EXTENSIONS_PERMISSIONS_CHROME_API_PERMISSIONS_H_

#include <vector>

#include "base/compiler_specific.h"
#include "extensions/common/permissions/permissions_provider.h"

namespace extensions {

class ChromeAPIPermissions : public PermissionsProvider {
 public:
  virtual std::vector<APIPermissionInfo*> GetAllPermissions() const OVERRIDE;
  virtual std::vector<PermissionsProvider::AliasInfo> GetAllAliases() const
      OVERRIDE;
};

}  

#endif  
