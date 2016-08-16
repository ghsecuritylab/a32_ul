// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_BROWSER_LOADER_RESOURCE_HANDLER_H_
#define CONTENT_BROWSER_LOADER_RESOURCE_HANDLER_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/sequenced_task_runner_helpers.h"
#include "base/threading/non_thread_safe.h"
#include "content/common/content_export.h"

class GURL;

namespace net {
class IOBuffer;
class URLRequest;
class URLRequestStatus;
struct RedirectInfo;
}  

namespace content {
class ResourceController;
class ResourceMessageFilter;
class ResourceRequestInfoImpl;
struct ResourceResponse;

class CONTENT_EXPORT ResourceHandler
    : public NON_EXPORTED_BASE(base::NonThreadSafe) {
 public:
  virtual ~ResourceHandler() {}

  
  virtual void SetController(ResourceController* controller);

  
  virtual bool OnUploadProgress(uint64 position, uint64 size) = 0;

  
  
  
  
  virtual bool OnRequestRedirected(const net::RedirectInfo& redirect_info,
                                   ResourceResponse* response,
                                   bool* defer) = 0;

  
  
  
  
  virtual bool OnResponseStarted(ResourceResponse* response, bool* defer) = 0;

  
  
  
  
  
  
  virtual bool OnWillStart(const GURL& url, bool* defer) = 0;

  
  
  
  
  
  virtual bool OnBeforeNetworkStart(const GURL& url, bool* defer) = 0;

  
  // size and address of the buffer where the data is to be written in its
  
  
  
  
  
  
  
  
  virtual bool OnWillRead(scoped_refptr<net::IOBuffer>* buf,
                          int* buf_size,
                          int min_size) = 0;

  // Data (*bytes_read bytes) was written into the buffer provided by
  
  
  
  
  virtual bool OnReadCompleted(int bytes_read, bool* defer) = 0;

  
  
  
  virtual void OnResponseCompleted(const net::URLRequestStatus& status,
                                   const std::string& security_info,
                                   bool* defer) = 0;

  
  
  
  
  virtual void OnDataDownloaded(int bytes_downloaded) = 0;

 protected:
  ResourceHandler(net::URLRequest* request);

  ResourceController* controller() const { return controller_; }
  net::URLRequest* request() const { return request_; }

  
  ResourceRequestInfoImpl* GetRequestInfo() const;
  int GetRequestID() const;
  ResourceMessageFilter* GetFilter() const;

 private:
  ResourceController* controller_;
  net::URLRequest* request_;
};

}  

#endif  