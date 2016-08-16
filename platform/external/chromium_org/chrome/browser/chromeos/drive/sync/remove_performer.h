// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_SYNC_REMOVE_PERFORMER_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_SYNC_REMOVE_PERFORMER_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "google_apis/drive/gdata_errorcode.h"

namespace base {
class SequencedTaskRunner;
}

namespace google_apis {
class FileResource;
}

namespace drive {

class JobScheduler;
class ResourceEntry;
struct ClientContext;

namespace file_system {
class OperationDelegate;
}  

namespace internal {

class EntryRevertPerformer;
class ResourceMetadata;

class RemovePerformer {
 public:
  RemovePerformer(base::SequencedTaskRunner* blocking_task_runner,
                  file_system::OperationDelegate* delegate,
                  JobScheduler* scheduler,
                  ResourceMetadata* metadata);
  ~RemovePerformer();

  
  
  
  void Remove(const std::string& local_id,
              const ClientContext& context,
              const FileOperationCallback& callback);

 private:
  
  void RemoveAfterGetResourceEntry(const ClientContext& context,
                                   const FileOperationCallback& callback,
                                   const ResourceEntry* entry,
                                   FileError error);

  
  void TrashResource(const ClientContext& context,
                     const FileOperationCallback& callback,
                     const std::string& resource_id,
                     const std::string& local_id);

  
  void TrashResourceAfterUpdateRemoteState(
      const ClientContext& context,
      const FileOperationCallback& callback,
      const std::string& local_id,
      google_apis::GDataErrorCode status);

  
  void UnparentResource(const ClientContext& context,
                        const FileOperationCallback& callback,
                        const std::string& resource_id,
                        const std::string& local_id);

  
  void UnparentResourceAfterGetFileResource(
      const ClientContext& context,
      const FileOperationCallback& callback,
      const std::string& local_id,
      google_apis::GDataErrorCode status,
      scoped_ptr<google_apis::FileResource> file_resource);

  
  void UnparentResourceAfterUpdateRemoteState(
      const FileOperationCallback& callback,
      const std::string& local_id,
      google_apis::GDataErrorCode status);

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;
  file_system::OperationDelegate* delegate_;
  JobScheduler* scheduler_;
  ResourceMetadata* metadata_;
  scoped_ptr<EntryRevertPerformer> entry_revert_performer_;

  
  
  base::WeakPtrFactory<RemovePerformer> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(RemovePerformer);
};

}  
}  

#endif  