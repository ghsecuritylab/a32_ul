// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_PEPPER_PEPPER_ISOLATED_FILE_SYSTEM_MESSAGE_FILTER_H_
#define CHROME_BROWSER_RENDERER_HOST_PEPPER_PEPPER_ISOLATED_FILE_SYSTEM_MESSAGE_FILTER_H_

#include <set>
#include <string>

#include "base/files/file_path.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/private/ppb_isolated_file_system_private.h"
#include "ppapi/host/resource_host.h"
#include "ppapi/host/resource_message_filter.h"
#include "url/gurl.h"

class Profile;

namespace content {
class BrowserPpapiHost;
}

namespace ppapi {
namespace host {
struct HostMessageContext;
}  
}  

namespace chrome {

class PepperIsolatedFileSystemMessageFilter
    : public ppapi::host::ResourceMessageFilter {
 public:
  static PepperIsolatedFileSystemMessageFilter* Create(
      PP_Instance instance,
      content::BrowserPpapiHost* host);

  
  virtual scoped_refptr<base::TaskRunner> OverrideTaskRunnerForMessage(
      const IPC::Message& msg) OVERRIDE;
  virtual int32_t OnResourceMessageReceived(
      const IPC::Message& msg,
      ppapi::host::HostMessageContext* context) OVERRIDE;

 private:
  PepperIsolatedFileSystemMessageFilter(int render_process_id,
                                        const base::FilePath& profile_directory,
                                        const GURL& document_url,
                                        ppapi::host::PpapiHost* ppapi_host_);

  virtual ~PepperIsolatedFileSystemMessageFilter();

  Profile* GetProfile();

  
  
  
  std::string CreateCrxFileSystem(Profile* profile);

  int32_t OnOpenFileSystem(ppapi::host::HostMessageContext* context,
                           PP_IsolatedFileSystemType_Private type);
  int32_t OpenCrxFileSystem(ppapi::host::HostMessageContext* context);
  int32_t OpenPluginPrivateFileSystem(ppapi::host::HostMessageContext* context);

  const int render_process_id_;
  
  const base::FilePath profile_directory_;
  const GURL document_url_;

  
  ppapi::host::PpapiHost* ppapi_host_;

  
  std::set<std::string> allowed_crxfs_origins_;

  DISALLOW_COPY_AND_ASSIGN(PepperIsolatedFileSystemMessageFilter);
};

}  

#endif  