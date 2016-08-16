// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DOWNLOADS_DOWNLOADS_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_DOWNLOADS_DOWNLOADS_API_H_

#include <set>
#include <string>

#include "base/files/file_path.h"
#include "base/scoped_observer.h"
#include "chrome/browser/download/all_download_item_notifier.h"
#include "chrome/browser/download/download_danger_prompt.h"
#include "chrome/browser/download/download_path_reservation_tracker.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/common/extensions/api/downloads.h"
#include "content/public/browser/download_manager.h"
#include "extensions/browser/event_router.h"
#include "extensions/browser/extension_registry_observer.h"
#include "extensions/browser/warning_set.h"

class DownloadFileIconExtractor;
class DownloadQuery;

namespace content {
class ResourceContext;
class ResourceDispatcherHost;
}

namespace extensions {
class ExtensionRegistry;
}


namespace download_extension_errors {

extern const char kEmptyFile[];
extern const char kFileAlreadyDeleted[];
extern const char kFileNotRemoved[];
extern const char kIconNotFound[];
extern const char kInvalidDangerType[];
extern const char kInvalidFilename[];
extern const char kInvalidFilter[];
extern const char kInvalidHeaderName[];
extern const char kInvalidHeaderValue[];
extern const char kInvalidHeaderUnsafe[];
extern const char kInvalidId[];
extern const char kInvalidOrderBy[];
extern const char kInvalidQueryLimit[];
extern const char kInvalidState[];
extern const char kInvalidURL[];
extern const char kInvisibleContext[];
extern const char kNotComplete[];
extern const char kNotDangerous[];
extern const char kNotInProgress[];
extern const char kNotResumable[];
extern const char kOpenPermission[];
extern const char kShelfDisabled[];
extern const char kShelfPermission[];
extern const char kTooManyListeners[];
extern const char kUnexpectedDeterminer[];
extern const char kUserGesture[];

}  

namespace extensions {

class DownloadedByExtension : public base::SupportsUserData::Data {
 public:
  static DownloadedByExtension* Get(content::DownloadItem* item);

  DownloadedByExtension(content::DownloadItem* item,
                        const std::string& id,
                        const std::string& name);

  const std::string& id() const { return id_; }
  const std::string& name() const { return name_; }

 private:
  static const char kKey[];

  std::string id_;
  std::string name_;

  DISALLOW_COPY_AND_ASSIGN(DownloadedByExtension);
};

class DownloadsDownloadFunction : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("downloads.download", DOWNLOADS_DOWNLOAD)
  DownloadsDownloadFunction();
  virtual bool RunAsync() OVERRIDE;

 protected:
  virtual ~DownloadsDownloadFunction();

 private:
  void OnStarted(const base::FilePath& creator_suggested_filename,
                 extensions::api::downloads::FilenameConflictAction
                     creator_conflict_action,
                 content::DownloadItem* item,
                 content::DownloadInterruptReason interrupt_reason);

  DISALLOW_COPY_AND_ASSIGN(DownloadsDownloadFunction);
};

class DownloadsSearchFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("downloads.search", DOWNLOADS_SEARCH)
  DownloadsSearchFunction();
  virtual bool RunSync() OVERRIDE;

 protected:
  virtual ~DownloadsSearchFunction();

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadsSearchFunction);
};

class DownloadsPauseFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("downloads.pause", DOWNLOADS_PAUSE)
  DownloadsPauseFunction();
  virtual bool RunSync() OVERRIDE;

 protected:
  virtual ~DownloadsPauseFunction();

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadsPauseFunction);
};

class DownloadsResumeFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("downloads.resume", DOWNLOADS_RESUME)
  DownloadsResumeFunction();
  virtual bool RunSync() OVERRIDE;

 protected:
  virtual ~DownloadsResumeFunction();

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadsResumeFunction);
};

class DownloadsCancelFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("downloads.cancel", DOWNLOADS_CANCEL)
  DownloadsCancelFunction();
  virtual bool RunSync() OVERRIDE;

 protected:
  virtual ~DownloadsCancelFunction();

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadsCancelFunction);
};

class DownloadsEraseFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("downloads.erase", DOWNLOADS_ERASE)
  DownloadsEraseFunction();
  virtual bool RunSync() OVERRIDE;

 protected:
  virtual ~DownloadsEraseFunction();

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadsEraseFunction);
};

class DownloadsRemoveFileFunction : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("downloads.removeFile", DOWNLOADS_REMOVEFILE)
  DownloadsRemoveFileFunction();
  virtual bool RunAsync() OVERRIDE;

 protected:
  virtual ~DownloadsRemoveFileFunction();

 private:
  void Done(bool success);

  DISALLOW_COPY_AND_ASSIGN(DownloadsRemoveFileFunction);
};

class DownloadsAcceptDangerFunction : public ChromeAsyncExtensionFunction {
 public:
  typedef base::Callback<void(DownloadDangerPrompt*)> OnPromptCreatedCallback;
  static void OnPromptCreatedForTesting(
      OnPromptCreatedCallback* callback) {
    on_prompt_created_ = callback;
  }

  DECLARE_EXTENSION_FUNCTION("downloads.acceptDanger", DOWNLOADS_ACCEPTDANGER)
  DownloadsAcceptDangerFunction();
  virtual bool RunAsync() OVERRIDE;

 protected:
  virtual ~DownloadsAcceptDangerFunction();
  void DangerPromptCallback(int download_id,
                            DownloadDangerPrompt::Action action);

 private:
  void PromptOrWait(int download_id, int retries);

  static OnPromptCreatedCallback* on_prompt_created_;
  DISALLOW_COPY_AND_ASSIGN(DownloadsAcceptDangerFunction);
};

class DownloadsShowFunction : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("downloads.show", DOWNLOADS_SHOW)
  DownloadsShowFunction();
  virtual bool RunAsync() OVERRIDE;

 protected:
  virtual ~DownloadsShowFunction();

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadsShowFunction);
};

class DownloadsShowDefaultFolderFunction : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION(
      "downloads.showDefaultFolder", DOWNLOADS_SHOWDEFAULTFOLDER)
  DownloadsShowDefaultFolderFunction();
  virtual bool RunAsync() OVERRIDE;

 protected:
  virtual ~DownloadsShowDefaultFolderFunction();

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadsShowDefaultFolderFunction);
};

class DownloadsOpenFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("downloads.open", DOWNLOADS_OPEN)
  DownloadsOpenFunction();
  virtual bool RunSync() OVERRIDE;

 protected:
  virtual ~DownloadsOpenFunction();

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadsOpenFunction);
};

class DownloadsSetShelfEnabledFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("downloads.setShelfEnabled",
                             DOWNLOADS_SETSHELFENABLED)
  DownloadsSetShelfEnabledFunction();
  virtual bool RunSync() OVERRIDE;

 protected:
  virtual ~DownloadsSetShelfEnabledFunction();

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadsSetShelfEnabledFunction);
};

class DownloadsDragFunction : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("downloads.drag", DOWNLOADS_DRAG)
  DownloadsDragFunction();
  virtual bool RunAsync() OVERRIDE;

 protected:
  virtual ~DownloadsDragFunction();

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadsDragFunction);
};

class DownloadsGetFileIconFunction : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("downloads.getFileIcon", DOWNLOADS_GETFILEICON)
  DownloadsGetFileIconFunction();
  virtual bool RunAsync() OVERRIDE;
  void SetIconExtractorForTesting(DownloadFileIconExtractor* extractor);

 protected:
  virtual ~DownloadsGetFileIconFunction();

 private:
  void OnIconURLExtracted(const std::string& url);
  base::FilePath path_;
  scoped_ptr<DownloadFileIconExtractor> icon_extractor_;
  DISALLOW_COPY_AND_ASSIGN(DownloadsGetFileIconFunction);
};

class ExtensionDownloadsEventRouter
    : public extensions::EventRouter::Observer,
      public extensions::ExtensionRegistryObserver,
      public AllDownloadItemNotifier::Observer {
 public:
  typedef base::Callback<void(
      const base::FilePath& changed_filename,
      DownloadPathReservationTracker::FilenameConflictAction)>
    FilenameChangedCallback;

  static void SetDetermineFilenameTimeoutSecondsForTesting(int s);

  
  
  static void DetermineFilenameInternal(
      const base::FilePath& filename,
      extensions::api::downloads::FilenameConflictAction conflict_action,
      const std::string& suggesting_extension_id,
      const base::Time& suggesting_install_time,
      const std::string& incumbent_extension_id,
      const base::Time& incumbent_install_time,
      std::string* winner_extension_id,
      base::FilePath* determined_filename,
      extensions::api::downloads::FilenameConflictAction*
        determined_conflict_action,
      extensions::WarningSet* warnings);

  
  
  
  
  
  
  static bool DetermineFilename(
      Profile* profile,
      bool include_incognito,
      const std::string& ext_id,
      int download_id,
      const base::FilePath& filename,
      extensions::api::downloads::FilenameConflictAction conflict_action,
      std::string* error);

  explicit ExtensionDownloadsEventRouter(
      Profile* profile, content::DownloadManager* manager);
  virtual ~ExtensionDownloadsEventRouter();

  void SetShelfEnabled(const extensions::Extension* extension, bool enabled);
  bool IsShelfEnabled() const;

  
  
  
  
  
  
  
  void OnDeterminingFilename(
      content::DownloadItem* item,
      const base::FilePath& suggested_path,
      const base::Closure& no_change,
      const FilenameChangedCallback& change);

  
  virtual void OnDownloadCreated(
      content::DownloadManager* manager,
      content::DownloadItem* download_item) OVERRIDE;
  virtual void OnDownloadUpdated(
      content::DownloadManager* manager,
      content::DownloadItem* download_item) OVERRIDE;
  virtual void OnDownloadRemoved(
      content::DownloadManager* manager,
      content::DownloadItem* download_item) OVERRIDE;

  
  virtual void OnListenerRemoved(
      const extensions::EventListenerInfo& details) OVERRIDE;

  
  struct DownloadsNotificationSource {
    std::string event_name;
    Profile* profile;
  };

 private:
  void DispatchEvent(
      const std::string& event_name,
      bool include_incognito,
      const extensions::Event::WillDispatchCallback& will_dispatch_callback,
      base::Value* json_arg);

  
  virtual void OnExtensionUnloaded(
      content::BrowserContext* browser_context,
      const extensions::Extension* extension,
      extensions::UnloadedExtensionInfo::Reason reason) OVERRIDE;

  Profile* profile_;
  AllDownloadItemNotifier notifier_;
  std::set<const extensions::Extension*> shelf_disabling_extensions_;

  
  ScopedObserver<extensions::ExtensionRegistry,
                 extensions::ExtensionRegistryObserver>
      extension_registry_observer_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionDownloadsEventRouter);
};

}  

#endif  