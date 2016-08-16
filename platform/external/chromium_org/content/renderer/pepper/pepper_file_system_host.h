// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_PEPPER_FILE_SYSTEM_HOST_H_
#define CONTENT_RENDERER_PEPPER_PEPPER_FILE_SYSTEM_HOST_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file.h"
#include "base/memory/weak_ptr.h"
#include "ppapi/c/pp_file_info.h"
#include "ppapi/c/private/ppb_isolated_file_system_private.h"
#include "ppapi/host/host_message_context.h"
#include "ppapi/host/resource_host.h"
#include "url/gurl.h"

namespace content {

class RendererPpapiHost;

class PepperFileSystemHost
    : public ppapi::host::ResourceHost,
      public base::SupportsWeakPtr<PepperFileSystemHost> {
 public:
  
  
  
  PepperFileSystemHost(RendererPpapiHost* host,
                       PP_Instance instance,
                       PP_Resource resource,
                       PP_FileSystemType type);
  
  
  
  PepperFileSystemHost(RendererPpapiHost* host,
                       PP_Instance instance,
                       PP_Resource resource,
                       const GURL& root_url,
                       PP_FileSystemType type);
  virtual ~PepperFileSystemHost();

  
  virtual int32_t OnResourceMessageReceived(
      const IPC::Message& msg,
      ppapi::host::HostMessageContext* context) OVERRIDE;
  virtual bool IsFileSystemHost() OVERRIDE;

  
  PP_FileSystemType GetType() const { return type_; }
  bool IsOpened() const { return opened_; }
  GURL GetRootUrl() const { return root_url_; }

 private:
  
  void DidOpenFileSystem(const std::string& name_unused, const GURL& root);
  void DidFailOpenFileSystem(base::File::Error error);

  int32_t OnHostMsgOpen(ppapi::host::HostMessageContext* context,
                        int64_t expected_size);
  int32_t OnHostMsgInitIsolatedFileSystem(
      ppapi::host::HostMessageContext* context,
      const std::string& fsid,
      PP_IsolatedFileSystemType_Private type);

  RendererPpapiHost* renderer_ppapi_host_;
  ppapi::host::ReplyMessageContext reply_context_;

  PP_FileSystemType type_;
  bool opened_;  
  GURL root_url_;
  bool called_open_;  

  base::WeakPtrFactory<PepperFileSystemHost> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PepperFileSystemHost);
};

}  

#endif  