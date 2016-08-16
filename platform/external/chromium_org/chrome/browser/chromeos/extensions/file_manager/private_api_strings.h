// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_STRINGS_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_FILE_MANAGER_PRIVATE_API_STRINGS_H_

#include "chrome/browser/extensions/chrome_extension_function.h"

namespace extensions {

class FileManagerPrivateGetStringsFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("fileManagerPrivate.getStrings",
                             FILEMANAGERPRIVATE_GETSTRINGS)

  FileManagerPrivateGetStringsFunction();

 protected:
  virtual ~FileManagerPrivateGetStringsFunction();

  
  virtual bool RunSync() OVERRIDE;
};

}  

#endif  