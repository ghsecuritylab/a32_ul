// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_FRAME_HOST_CROSS_SITE_TRANSFERRING_REQUEST_H_
#define CONTENT_BROWSER_FRAME_HOST_CROSS_SITE_TRANSFERRING_REQUEST_H_

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "content/public/browser/global_request_id.h"

namespace content {

class CrossSiteResourceHandler;

class CONTENT_EXPORT CrossSiteTransferringRequest {
 public:
  explicit CrossSiteTransferringRequest(GlobalRequestID global_request_id);
  ~CrossSiteTransferringRequest();

  
  
  void ReleaseRequest();

  GlobalRequestID request_id() const { return global_request_id_; }

 private:
  
  
  
  GlobalRequestID global_request_id_;

  DISALLOW_COPY_AND_ASSIGN(CrossSiteTransferringRequest);
};

}  

#endif  