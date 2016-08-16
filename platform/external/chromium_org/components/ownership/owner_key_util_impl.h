// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_OWNERSHIP_OWNER_KEY_UTIL_IMPL_H_
#define COMPONENTS_OWNERSHIP_OWNER_KEY_UTIL_IMPL_H_

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/macros.h"
#include "components/ownership/owner_key_util.h"
#include "components/ownership/ownership_export.h"

namespace ownership {

class OWNERSHIP_EXPORT OwnerKeyUtilImpl : public OwnerKeyUtil {
 public:
  explicit OwnerKeyUtilImpl(const base::FilePath& public_key_file);

  
  virtual bool ImportPublicKey(std::vector<uint8>* output) OVERRIDE;
#if defined(USE_NSS)
  virtual crypto::RSAPrivateKey* FindPrivateKeyInSlot(
      const std::vector<uint8>& key,
      PK11SlotInfo* slot) OVERRIDE;
#endif  
  virtual bool IsPublicKeyPresent() OVERRIDE;

 private:
  virtual ~OwnerKeyUtilImpl();

  
  base::FilePath public_key_file_;

  DISALLOW_COPY_AND_ASSIGN(OwnerKeyUtilImpl);
};

}  

#endif  