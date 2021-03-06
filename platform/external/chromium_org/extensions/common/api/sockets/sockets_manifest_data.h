// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_API_SOCKETS_SOCKETS_MANIFEST_DATA_H_
#define EXTENSIONS_COMMON_API_SOCKETS_SOCKETS_MANIFEST_DATA_H_

#include <vector>

#include "base/strings/string16.h"
#include "extensions/common/extension.h"
#include "extensions/common/manifest_handler.h"

namespace content {
struct SocketPermissionRequest;
}

namespace extensions {
class SocketsManifestPermission;
}

namespace extensions {

class SocketsManifestData : public Extension::ManifestData {
 public:
  explicit SocketsManifestData(
      scoped_ptr<SocketsManifestPermission> permission);
  virtual ~SocketsManifestData();

  
  
  static SocketsManifestData* Get(const Extension* extension);

  static bool CheckRequest(const Extension* extension,
                           const content::SocketPermissionRequest& request);

  
  
  static scoped_ptr<SocketsManifestData> FromValue(const base::Value& value,
                                                   base::string16* error);

  const SocketsManifestPermission* permission() const {
    return permission_.get();
  }

 private:
  scoped_ptr<SocketsManifestPermission> permission_;
};

}  

#endif  
