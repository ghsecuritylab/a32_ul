// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_CRX_INSTALLER_H_
#define CHROME_BROWSER_EXTENSIONS_CRX_INSTALLER_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/version.h"
#include "chrome/browser/extensions/blacklist.h"
#include "chrome/browser/extensions/extension_install_checker.h"
#include "chrome/browser/extensions/extension_install_prompt.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/extensions/sandboxed_unpacker.h"
#include "chrome/browser/extensions/webstore_installer.h"
#include "chrome/common/extensions/extension_constants.h"
#include "extensions/browser/install_flag.h"
#include "extensions/common/extension.h"
#include "extensions/common/manifest.h"
#include "sync/api/string_ordinal.h"

class ExtensionService;
class ExtensionServiceTest;
class SkBitmap;
struct WebApplicationInfo;

namespace base {
class SequencedTaskRunner;
}

namespace extensions {
class CrxInstallerError;
class ExtensionUpdaterTest;
class RequirementsChecker;

class CrxInstaller
    : public SandboxedUnpackerClient,
      public ExtensionInstallPrompt::Delegate {
 public:
  
  enum OffStoreInstallAllowReason {
    OffStoreInstallDisallowed,
    OffStoreInstallAllowedFromSettingsPage,
    OffStoreInstallAllowedBecausePref,
    OffStoreInstallAllowedInTest,
    NumOffStoreInstallAllowReasons
  };

  
  
  
  static scoped_refptr<CrxInstaller> CreateSilent(ExtensionService* service);

  
  static scoped_refptr<CrxInstaller> Create(
      ExtensionService* service,
      scoped_ptr<ExtensionInstallPrompt> client);

  
  
  static scoped_refptr<CrxInstaller> Create(
      ExtensionService* service,
      scoped_ptr<ExtensionInstallPrompt> client,
      const WebstoreInstaller::Approval* approval);

  
  void InstallCrx(const base::FilePath& source_file);

  
  void InstallUserScript(const base::FilePath& source_file,
                         const GURL& download_url);

  
  void InstallWebApp(const WebApplicationInfo& web_app);

  
  virtual void InstallUIProceed() OVERRIDE;
  virtual void InstallUIAbort(bool user_initiated) OVERRIDE;

  int creation_flags() const { return creation_flags_; }
  void set_creation_flags(int val) { creation_flags_ = val; }

  const base::FilePath& source_file() const { return source_file_; }

  Manifest::Location install_source() const {
    return install_source_;
  }
  void set_install_source(Manifest::Location source) {
    install_source_ = source;
  }

  const std::string& expected_id() const { return expected_id_; }
  void set_expected_id(const std::string& val) { expected_id_ = val; }

  void set_expected_version(const Version& val) {
    expected_version_.reset(new Version(val));
    expected_version_strict_checking_ = true;
  }

  bool delete_source() const { return delete_source_; }
  void set_delete_source(bool val) { delete_source_ = val; }

  bool allow_silent_install() const { return allow_silent_install_; }
  void set_allow_silent_install(bool val) { allow_silent_install_ = val; }

  bool grant_permissions() const { return grant_permissions_; }
  void set_grant_permissions(bool val) { grant_permissions_ = val; }

  bool is_gallery_install() const {
    return (creation_flags_ & Extension::FROM_WEBSTORE) > 0;
  }
  void set_is_gallery_install(bool val) {
    if (val)
      creation_flags_ |= Extension::FROM_WEBSTORE;
    else
      creation_flags_ &= ~Extension::FROM_WEBSTORE;
  }

  
  
  void set_apps_require_extension_mime_type(
      bool apps_require_extension_mime_type) {
    apps_require_extension_mime_type_ = apps_require_extension_mime_type;
  }

  void set_original_mime_type(const std::string& original_mime_type) {
    original_mime_type_ = original_mime_type;
  }

  extension_misc::CrxInstallCause install_cause() const {
    return install_cause_;
  }
  void set_install_cause(extension_misc::CrxInstallCause install_cause) {
    install_cause_ = install_cause;
  }

  OffStoreInstallAllowReason off_store_install_allow_reason() const {
    return off_store_install_allow_reason_;
  }
  void set_off_store_install_allow_reason(OffStoreInstallAllowReason reason) {
    off_store_install_allow_reason_ = reason;
  }

  void set_page_ordinal(const syncer::StringOrdinal& page_ordinal) {
    page_ordinal_ = page_ordinal;
  }

  void set_error_on_unsupported_requirements(bool val) {
    error_on_unsupported_requirements_ = val;
  }

  void set_install_immediately(bool val) {
    set_install_flag(kInstallFlagInstallImmediately, val);
  }
  void set_is_ephemeral(bool val) {
    set_install_flag(kInstallFlagIsEphemeral, val);
  }
  void set_do_not_sync(bool val) {
    set_install_flag(kInstallFlagDoNotSync, val);
  }

  bool did_handle_successfully() const { return did_handle_successfully_; }

  Profile* profile() { return install_checker_.profile(); }

  const Extension* extension() { return install_checker_.extension().get(); }

  const std::string& current_version() const { return current_version_; }

 private:
  friend class ::ExtensionServiceTest;
  friend class ExtensionUpdaterTest;
  friend class ExtensionCrxInstallerTest;

  CrxInstaller(base::WeakPtr<ExtensionService> service_weak,
               scoped_ptr<ExtensionInstallPrompt> client,
               const WebstoreInstaller::Approval* approval);
  virtual ~CrxInstaller();

  
  void ConvertUserScriptOnFileThread();

  
  void ConvertWebAppOnFileThread(const WebApplicationInfo& web_app);

  
  
  CrxInstallerError AllowInstall(const Extension* extension);

  
  virtual void OnUnpackFailure(const base::string16& error_message) OVERRIDE;
  virtual void OnUnpackSuccess(const base::FilePath& temp_dir,
                               const base::FilePath& extension_dir,
                               const base::DictionaryValue* original_manifest,
                               const Extension* extension,
                               const SkBitmap& install_icon) OVERRIDE;

  
  
  void CheckInstall();

  
  void OnInstallChecksComplete(int failed_checks);

  
  void OnBlacklistChecked(
      extensions::BlacklistState blacklist_state);

  
  void ConfirmInstall();

  
  
  void CompleteInstall();

  
  
  void ReloadExtensionAfterInstall(const base::FilePath& version_dir);

  
  void ReportFailureFromFileThread(const CrxInstallerError& error);
  void ReportFailureFromUIThread(const CrxInstallerError& error);
  void ReportSuccessFromFileThread();
  void ReportSuccessFromUIThread();
  void NotifyCrxInstallBegin();
  void NotifyCrxInstallComplete(bool success);

  
  void CleanupTempFiles();

  
  
  void CheckUpdateFromSettingsPage();

  
  
  void ConfirmReEnable();

  void set_install_flag(int flag, bool val) {
    if (val)
      install_flags_ |= flag;
    else
      install_flags_ &= ~flag;
  }

  
  base::FilePath source_file_;

  
  GURL download_url_;

  
  const base::FilePath install_directory_;

  
  
  
  Manifest::Location install_source_;

  
  
  
  bool approved_;

  
  
  std::string expected_id_;

  
  
  
  scoped_ptr<Manifest> expected_manifest_;

  
  
  WebstoreInstaller::ManifestCheckLevel expected_manifest_check_level_;

  
  
  
  
  scoped_ptr<Version> expected_version_;

  
  
  
  bool expected_version_strict_checking_;

  
  
  
  bool extensions_enabled_;

  
  
  bool delete_source_;

  
  
  
  bool create_app_shortcut_;

  
  syncer::StringOrdinal page_ordinal_;

  
  
  scoped_ptr<Manifest> original_manifest_;

  
  
  std::string current_version_;

  
  scoped_ptr<SkBitmap> install_icon_;

  
  
  base::FilePath temp_dir_;

  
  base::WeakPtr<ExtensionService> service_weak_;

  
  
  
  
  ExtensionInstallPrompt* client_;

  
  
  base::FilePath unpacked_extension_root_;

  
  
  bool apps_require_extension_mime_type_;

  
  
  
  bool allow_silent_install_;

  
  
  bool grant_permissions_;

  
  
  std::string original_mime_type_;

  
  
  extension_misc::CrxInstallCause install_cause_;

  
  
  int creation_flags_;

  
  OffStoreInstallAllowReason off_store_install_allow_reason_;

  
  
  
  
  
  bool did_handle_successfully_;

  
  
  
  bool error_on_unsupported_requirements_;

  
  scoped_refptr<base::SequencedTaskRunner> installer_task_runner_;

  
  ExtensionInstallPrompt::ShowDialogCallback show_dialog_callback_;

  
  
  bool update_from_settings_page_;

  
  int install_flags_;

  
  ExtensionInstallChecker install_checker_;

  DISALLOW_COPY_AND_ASSIGN(CrxInstaller);
};

}  

#endif  
