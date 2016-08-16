// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FILEAPI_FILE_ACCESS_PERMISSIONS_H_
#define CHROME_BROWSER_CHROMEOS_FILEAPI_FILE_ACCESS_PERMISSIONS_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/synchronization/lock.h"

namespace chromeos {

class FileAccessPermissions {
 public:
  FileAccessPermissions();
  virtual ~FileAccessPermissions();

  
  void GrantFullAccessPermission(const std::string& extension_id);
  
  void GrantAccessPermission(const std::string& extension_id,
                             const base::FilePath& path);
  
  bool HasAccessPermission(const std::string& extension_id,
                           const base::FilePath& path) const;
  
  void RevokePermissions(const std::string& extension_id);

 private:
  typedef std::set<base::FilePath> PathSet;
  typedef std::map<std::string, PathSet> PathAccessMap;

  mutable base::Lock lock_;  
  PathAccessMap path_map_;

  DISALLOW_COPY_AND_ASSIGN(FileAccessPermissions);
};

}  

#endif  