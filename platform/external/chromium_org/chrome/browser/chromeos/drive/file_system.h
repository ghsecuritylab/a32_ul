// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_SYSTEM_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/chromeos/drive/change_list_loader_observer.h"
#include "chrome/browser/chromeos/drive/file_system/operation_delegate.h"
#include "chrome/browser/chromeos/drive/file_system_interface.h"
#include "google_apis/drive/gdata_errorcode.h"

class PrefService;

namespace base {
class SequencedTaskRunner;
}  

namespace google_apis {
class AboutResource;
class ResourceEntry;
}  

namespace drive {

class DriveServiceInterface;
class EventLogger;
class FileCacheEntry;
class FileSystemObserver;
class JobScheduler;

namespace internal {
class AboutResourceLoader;
class ChangeListLoader;
class DirectoryLoader;
class FileCache;
class LoaderController;
class ResourceMetadata;
class SyncClient;
}  

namespace file_system {
class CopyOperation;
class CreateDirectoryOperation;
class CreateFileOperation;
class DownloadOperation;
class GetFileForSavingOperation;
class MoveOperation;
class OpenFileOperation;
class RemoveOperation;
class SearchOperation;
class TouchOperation;
class TruncateOperation;
}  

class FileSystem : public FileSystemInterface,
                   public internal::ChangeListLoaderObserver,
                   public file_system::OperationDelegate {
 public:
  FileSystem(PrefService* pref_service,
             EventLogger* logger,
             internal::FileCache* cache,
             DriveServiceInterface* drive_service,
             JobScheduler* scheduler,
             internal::ResourceMetadata* resource_metadata,
             base::SequencedTaskRunner* blocking_task_runner,
             const base::FilePath& temporary_file_directory);
  virtual ~FileSystem();

  
  virtual void AddObserver(FileSystemObserver* observer) OVERRIDE;
  virtual void RemoveObserver(FileSystemObserver* observer) OVERRIDE;
  virtual void CheckForUpdates() OVERRIDE;
  virtual void Search(const std::string& search_query,
                      const GURL& next_link,
                      const SearchCallback& callback) OVERRIDE;
  virtual void SearchMetadata(const std::string& query,
                              int options,
                              int at_most_num_matches,
                              const SearchMetadataCallback& callback) OVERRIDE;
  virtual void TransferFileFromLocalToRemote(
      const base::FilePath& local_src_file_path,
      const base::FilePath& remote_dest_file_path,
      const FileOperationCallback& callback) OVERRIDE;
  virtual void OpenFile(const base::FilePath& file_path,
                        OpenMode open_mode,
                        const std::string& mime_type,
                        const OpenFileCallback& callback) OVERRIDE;
  virtual void Copy(const base::FilePath& src_file_path,
                    const base::FilePath& dest_file_path,
                    bool preserve_last_modified,
                    const FileOperationCallback& callback) OVERRIDE;
  virtual void Move(const base::FilePath& src_file_path,
                    const base::FilePath& dest_file_path,
                    const FileOperationCallback& callback) OVERRIDE;
  virtual void Remove(const base::FilePath& file_path,
                      bool is_recursive,
                      const FileOperationCallback& callback) OVERRIDE;
  virtual void CreateDirectory(const base::FilePath& directory_path,
                               bool is_exclusive,
                               bool is_recursive,
                               const FileOperationCallback& callback) OVERRIDE;
  virtual void CreateFile(const base::FilePath& file_path,
                          bool is_exclusive,
                          const std::string& mime_type,
                          const FileOperationCallback& callback) OVERRIDE;
  virtual void TouchFile(const base::FilePath& file_path,
                         const base::Time& last_access_time,
                         const base::Time& last_modified_time,
                         const FileOperationCallback& callback) OVERRIDE;
  virtual void TruncateFile(const base::FilePath& file_path,
                            int64 length,
                            const FileOperationCallback& callback) OVERRIDE;
  virtual void Pin(const base::FilePath& file_path,
                   const FileOperationCallback& callback) OVERRIDE;
  virtual void Unpin(const base::FilePath& file_path,
                     const FileOperationCallback& callback) OVERRIDE;
  virtual void GetFile(const base::FilePath& file_path,
                             const GetFileCallback& callback) OVERRIDE;
  virtual void GetFileForSaving(const base::FilePath& file_path,
                                      const GetFileCallback& callback) OVERRIDE;
  virtual base::Closure GetFileContent(
      const base::FilePath& file_path,
      const GetFileContentInitializedCallback& initialized_callback,
      const google_apis::GetContentCallback& get_content_callback,
      const FileOperationCallback& completion_callback) OVERRIDE;
  virtual void GetResourceEntry(
      const base::FilePath& file_path,
      const GetResourceEntryCallback& callback) OVERRIDE;
  virtual void ReadDirectory(
      const base::FilePath& directory_path,
      const ReadDirectoryEntriesCallback& entries_callback,
      const FileOperationCallback& completion_callback) OVERRIDE;
  virtual void GetAvailableSpace(
      const GetAvailableSpaceCallback& callback) OVERRIDE;
  virtual void GetShareUrl(
      const base::FilePath& file_path,
      const GURL& embed_origin,
      const GetShareUrlCallback& callback) OVERRIDE;
  virtual void GetMetadata(
      const GetFilesystemMetadataCallback& callback) OVERRIDE;
  virtual void MarkCacheFileAsMounted(
      const base::FilePath& drive_file_path,
      const MarkMountedCallback& callback) OVERRIDE;
  virtual void MarkCacheFileAsUnmounted(
      const base::FilePath& cache_file_path,
      const FileOperationCallback& callback) OVERRIDE;
  virtual void AddPermission(const base::FilePath& drive_file_path,
                             const std::string& email,
                             google_apis::drive::PermissionRole role,
                             const FileOperationCallback& callback) OVERRIDE;
  virtual void Reset(const FileOperationCallback& callback) OVERRIDE;
  virtual void GetPathFromResourceId(const std::string& resource_id,
                                     const GetFilePathCallback& callback)
      OVERRIDE;

  
  virtual void OnFileChangedByOperation(
      const FileChange& changed_files) OVERRIDE;
  virtual void OnEntryUpdatedByOperation(const std::string& local_id) OVERRIDE;
  virtual void OnDriveSyncError(file_system::DriveSyncErrorType type,
                                const std::string& local_id) OVERRIDE;
  virtual bool WaitForSyncComplete(
      const std::string& local_id,
      const FileOperationCallback& callback) OVERRIDE;

  
  
  virtual void OnDirectoryReloaded(
      const base::FilePath& directory_path) OVERRIDE;
  virtual void OnFileChanged(const FileChange& changed_files) OVERRIDE;
  virtual void OnLoadFromServerComplete() OVERRIDE;
  virtual void OnInitialLoadComplete() OVERRIDE;

  
  internal::ChangeListLoader* change_list_loader_for_testing() {
    return change_list_loader_.get();
  }
  internal::SyncClient* sync_client_for_testing() { return sync_client_.get(); }

 private:
  struct CreateDirectoryParams;

  
  
  void ResetComponents();

  
  
  void CreateDirectoryAfterRead(const CreateDirectoryParams& params,
                                FileError error);

  void FinishPin(const FileOperationCallback& callback,
                 const std::string* local_id,
                 FileError error);

  void FinishUnpin(const FileOperationCallback& callback,
                   const std::string* local_id,
                   FileError error);

  
  void OnGetAboutResource(
      const GetAvailableSpaceCallback& callback,
      google_apis::GDataErrorCode status,
      scoped_ptr<google_apis::AboutResource> about_resource);

  
  
  void OnUpdateChecked(FileError error);

  
  
  void GetResourceEntryAfterRead(const base::FilePath& file_path,
                                 const GetResourceEntryCallback& callback,
                                 FileError error);

  
  
  void GetShareUrlAfterGetResourceEntry(const base::FilePath& file_path,
                                        const GURL& embed_origin,
                                        const GetShareUrlCallback& callback,
                                        ResourceEntry* entry,
                                        FileError error);
  void OnGetResourceEntryForGetShareUrl(const GetShareUrlCallback& callback,
                                        google_apis::GDataErrorCode status,
                                        const GURL& share_url);
  
  void AddPermissionAfterGetResourceEntry(
      const std::string& email,
      google_apis::drive::PermissionRole role,
      const FileOperationCallback& callback,
      ResourceEntry* entry,
      FileError error);

  
  virtual void OnDriveSyncErrorAfterGetFilePath(
      file_system::DriveSyncErrorType type,
      const base::FilePath* file_path,
      FileError error);

  
  PrefService* pref_service_;

  
  EventLogger* logger_;
  internal::FileCache* cache_;
  DriveServiceInterface* drive_service_;
  JobScheduler* scheduler_;
  internal::ResourceMetadata* resource_metadata_;

  
  base::Time last_update_check_time_;

  
  FileError last_update_check_error_;

  
  scoped_ptr<internal::AboutResourceLoader> about_resource_loader_;

  
  scoped_ptr<internal::LoaderController> loader_controller_;

  
  scoped_ptr<internal::ChangeListLoader> change_list_loader_;

  scoped_ptr<internal::DirectoryLoader> directory_loader_;

  scoped_ptr<internal::SyncClient> sync_client_;

  ObserverList<FileSystemObserver> observers_;

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;

  base::FilePath temporary_file_directory_;

  
  scoped_ptr<file_system::CopyOperation> copy_operation_;
  scoped_ptr<file_system::CreateDirectoryOperation> create_directory_operation_;
  scoped_ptr<file_system::CreateFileOperation> create_file_operation_;
  scoped_ptr<file_system::MoveOperation> move_operation_;
  scoped_ptr<file_system::OpenFileOperation> open_file_operation_;
  scoped_ptr<file_system::RemoveOperation> remove_operation_;
  scoped_ptr<file_system::TouchOperation> touch_operation_;
  scoped_ptr<file_system::TruncateOperation> truncate_operation_;
  scoped_ptr<file_system::DownloadOperation> download_operation_;
  scoped_ptr<file_system::SearchOperation> search_operation_;
  scoped_ptr<file_system::GetFileForSavingOperation>
      get_file_for_saving_operation_;

  
  
  base::WeakPtrFactory<FileSystem> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(FileSystem);
};

}  

#endif  
