// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_COMPONENT_UPDATER_TEST_TEST_INSTALLER_H_
#define CHROME_BROWSER_COMPONENT_UPDATER_TEST_TEST_INSTALLER_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "components/component_updater/component_updater_service.h"

namespace base {
class DictionaryValue;
}

namespace component_updater {

class TestInstaller : public ComponentInstaller {
 public:
  TestInstaller();

  virtual void OnUpdateError(int error) OVERRIDE;

  virtual bool Install(const base::DictionaryValue& manifest,
                       const base::FilePath& unpack_path) OVERRIDE;

  virtual bool GetInstalledFile(const std::string& file,
                                base::FilePath* installed_file) OVERRIDE;

  int error() const;

  int install_count() const;

 protected:
  int error_;
  int install_count_;
};

class ReadOnlyTestInstaller : public TestInstaller {
 public:
  explicit ReadOnlyTestInstaller(const base::FilePath& installed_path);

  virtual ~ReadOnlyTestInstaller();

  virtual bool GetInstalledFile(const std::string& file,
                                base::FilePath* installed_file) OVERRIDE;

 private:
  base::FilePath install_directory_;
};

class VersionedTestInstaller : public TestInstaller {
 public:
  VersionedTestInstaller();

  virtual ~VersionedTestInstaller();

  virtual bool Install(const base::DictionaryValue& manifest,
                       const base::FilePath& unpack_path) OVERRIDE;

  virtual bool GetInstalledFile(const std::string& file,
                                base::FilePath* installed_file) OVERRIDE;

 private:
  base::FilePath install_directory_;
  Version current_version_;
};

}  

#endif  