// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_LOCAL_LOCAL_FILE_SYNC_SERVICE_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_LOCAL_LOCAL_FILE_SYNC_SERVICE_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/sync_file_system/local/local_origin_change_observer.h"
#include "chrome/browser/sync_file_system/remote_change_processor.h"
#include "chrome/browser/sync_file_system/sync_callbacks.h"
#include "chrome/browser/sync_file_system/sync_status_code.h"

class GURL;
class Profile;

namespace storage {
class FileSystemContext;
}

namespace leveldb {
class Env;
}

namespace storage {
class ScopedFile;
}

namespace sync_file_system {

class FileChange;
class LocalChangeProcessor;
class LocalFileSyncContext;
struct LocalFileSyncInfo;

class LocalFileSyncService
    : public RemoteChangeProcessor,
      public LocalOriginChangeObserver,
      public base::SupportsWeakPtr<LocalFileSyncService> {
 public:
  typedef base::Callback<LocalChangeProcessor*(const GURL& origin)>
      GetLocalChangeProcessorCallback;

  class Observer {
   public:
    Observer() {}
    virtual ~Observer() {}

    
    
    
    
    virtual void OnLocalChangeAvailable(int64 pending_changes_hint) = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(Observer);
  };

  typedef base::Callback<void(SyncStatusCode status,
                              bool has_pending_changes)>
      HasPendingLocalChangeCallback;

  static scoped_ptr<LocalFileSyncService> Create(Profile* profile);
  static scoped_ptr<LocalFileSyncService> CreateForTesting(
      Profile* profile,
      leveldb::Env* env_override);
  virtual ~LocalFileSyncService();

  void Shutdown();

  void MaybeInitializeFileSystemContext(
      const GURL& app_origin,
      storage::FileSystemContext* file_system_context,
      const SyncStatusCallback& callback);

  void AddChangeObserver(Observer* observer);

  
  
  
  
  
  
  void RegisterURLForWaitingSync(const storage::FileSystemURL& url,
                                 const base::Closure& on_syncable_callback);

  
  
  
  
  void ProcessLocalChange(const SyncFileCallback& callback);

  
  
  
  
  void SetLocalChangeProcessor(LocalChangeProcessor* local_change_processor);

  
  
  
  
  
  
  
  
  void SetLocalChangeProcessorCallback(
      const GetLocalChangeProcessorCallback& get_local_change_processor);

  
  
  void HasPendingLocalChanges(const storage::FileSystemURL& url,
                              const HasPendingLocalChangeCallback& callback);

  void PromoteDemotedChanges(const base::Closure& callback);

  
  virtual void GetLocalFileMetadata(const storage::FileSystemURL& url,
                                    const SyncFileMetadataCallback& callback);

  
  virtual void PrepareForProcessRemoteChange(
      const storage::FileSystemURL& url,
      const PrepareChangeCallback& callback) OVERRIDE;
  virtual void ApplyRemoteChange(const FileChange& change,
                                 const base::FilePath& local_path,
                                 const storage::FileSystemURL& url,
                                 const SyncStatusCallback& callback) OVERRIDE;
  virtual void FinalizeRemoteSync(
      const storage::FileSystemURL& url,
      bool clear_local_changes,
      const base::Closure& completion_callback) OVERRIDE;
  virtual void RecordFakeLocalChange(
      const storage::FileSystemURL& url,
      const FileChange& change,
      const SyncStatusCallback& callback) OVERRIDE;

  
  virtual void OnChangesAvailableInOrigins(
      const std::set<GURL>& origins) OVERRIDE;

  
  
  
  void SetOriginEnabled(const GURL& origin, bool enabled);

 private:
  typedef std::map<GURL, storage::FileSystemContext*> OriginToContext;
  friend class OriginChangeMapTest;

  class OriginChangeMap {
   public:
    typedef std::map<GURL, int64> Map;

    OriginChangeMap();
    ~OriginChangeMap();

    
    
    
    bool NextOriginToProcess(GURL* origin);

    int64 GetTotalChangeCount() const;

    
    void SetOriginChangeCount(const GURL& origin, int64 changes);

    void SetOriginEnabled(const GURL& origin, bool enabled);

   private:
    
    Map change_count_map_;
    Map::iterator next_;

    
    std::set<GURL> disabled_origins_;
  };

  LocalFileSyncService(Profile* profile, leveldb::Env* env_override);

  void DidInitializeFileSystemContext(
      const GURL& app_origin,
      storage::FileSystemContext* file_system_context,
      const SyncStatusCallback& callback,
      SyncStatusCode status);
  void DidInitializeForRemoteSync(
      const storage::FileSystemURL& url,
      storage::FileSystemContext* file_system_context,
      const PrepareChangeCallback& callback,
      SyncStatusCode status);

  
  void RunLocalSyncCallback(SyncStatusCode status,
                            const storage::FileSystemURL& url);

  
  void DidApplyRemoteChange(
      const SyncStatusCallback& callback,
      SyncStatusCode status);

  
  void DidGetFileForLocalSync(SyncStatusCode status,
                              const LocalFileSyncInfo& sync_file_info,
                              storage::ScopedFile snapshot);
  void ProcessNextChangeForURL(storage::ScopedFile snapshot,
                               const LocalFileSyncInfo& sync_file_info,
                               const FileChange& last_change,
                               const FileChangeList& changes,
                               SyncStatusCode status);

  
  LocalChangeProcessor* GetLocalChangeProcessor(
      const storage::FileSystemURL& url);

  Profile* profile_;

  scoped_refptr<LocalFileSyncContext> sync_context_;

  
  
  OriginToContext origin_to_contexts_;

  
  
  
  std::set<GURL> pending_origins_with_changes_;

  OriginChangeMap origin_change_map_;

  
  
  SyncFileCallback local_sync_callback_;

  LocalChangeProcessor* local_change_processor_;
  GetLocalChangeProcessorCallback get_local_change_processor_;

  ObserverList<Observer> change_observers_;

  DISALLOW_COPY_AND_ASSIGN(LocalFileSyncService);
};

}  

#endif  