// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_UTIL_BROWSER_DISTRIBUTION_H_
#define CHROME_INSTALLER_UTIL_BROWSER_DISTRIBUTION_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/version.h"
#include "chrome/installer/util/util_constants.h"

#if defined(OS_WIN)
#include <windows.h>  
#endif

class AppRegistrationData;

class BrowserDistribution {
 public:
  enum Type {
    CHROME_BROWSER,
    CHROME_FRAME,
    CHROME_BINARIES,
    CHROME_APP_HOST,
    NUM_TYPES
  };

  enum ShortcutType {
    SHORTCUT_CHROME,
    SHORTCUT_CHROME_ALTERNATE,
    SHORTCUT_APP_LAUNCHER
  };

  enum Subfolder {
    SUBFOLDER_CHROME,
    SUBFOLDER_APPS,
  };

  enum DefaultBrowserControlPolicy {
    DEFAULT_BROWSER_UNSUPPORTED,
    DEFAULT_BROWSER_OS_CONTROL_ONLY,
    DEFAULT_BROWSER_FULL_CONTROL
  };

  virtual ~BrowserDistribution();

  static BrowserDistribution* GetDistribution();

  static BrowserDistribution* GetSpecificDistribution(Type type);

  Type GetType() const { return type_; }

  
  const AppRegistrationData& GetAppRegistrationData() const;
  base::string16 GetAppGuid() const;
  base::string16 GetStateKey() const;
  base::string16 GetStateMediumKey() const;
  base::string16 GetVersionKey() const;

  virtual void DoPostUninstallOperations(
      const Version& version,
      const base::FilePath& local_data_path,
      const base::string16& distribution_data);

  
  virtual base::string16 GetActiveSetupGuid();

  
  
  
  
  
  virtual base::string16 GetBaseAppName();

  
  virtual base::string16 GetDisplayName();

  
  
  virtual base::string16 GetShortcutName(ShortcutType shortcut_type);

  
  
  virtual int GetIconIndex(ShortcutType shortcut_type);

  
  virtual base::string16 GetIconFilename();

  
  
  
  virtual base::string16 GetStartMenuShortcutSubfolder(
      Subfolder subfolder_type);

  
  
  
  
  virtual base::string16 GetBaseAppId();

  
  
  
  
  
  
  
  virtual base::string16 GetBrowserProgIdPrefix();

  
  virtual base::string16 GetBrowserProgIdDesc();

  virtual base::string16 GetInstallSubDir();

  virtual base::string16 GetPublisherName();

  virtual base::string16 GetAppDescription();

  virtual base::string16 GetLongAppDescription();

  virtual std::string GetSafeBrowsingName();

  virtual std::string GetNetworkStatsServer() const;

#if defined(OS_WIN)
  virtual base::string16 GetDistributionData(HKEY root_key);
#endif

  virtual base::string16 GetUninstallLinkName();

  virtual base::string16 GetUninstallRegPath();

  
  
  virtual DefaultBrowserControlPolicy GetDefaultBrowserControlPolicy();

  virtual bool CanCreateDesktopShortcuts();

  virtual bool GetChromeChannel(base::string16* channel);

  
  
  
  virtual bool GetCommandExecuteImplClsid(base::string16* handler_class_uuid);

  
  virtual bool AppHostIsSupported();

  virtual void UpdateInstallStatus(bool system_install,
      installer::ArchiveType archive_type,
      installer::InstallStatus install_status);

  
  
  virtual bool ShouldSetExperimentLabels();

  virtual bool HasUserExperiments();

 protected:
  BrowserDistribution(Type type, scoped_ptr<AppRegistrationData> app_reg_data);

  template<class DistributionClass>
  static BrowserDistribution* GetOrCreateBrowserDistribution(
      BrowserDistribution** dist);

  const Type type_;

  scoped_ptr<AppRegistrationData> app_reg_data_;

 private:
  BrowserDistribution();

  DISALLOW_COPY_AND_ASSIGN(BrowserDistribution);
};

#endif  