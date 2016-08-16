// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_BROWSER_AW_REQUEST_INTERCEPTOR_H_
#define ANDROID_WEBVIEW_BROWSER_AW_REQUEST_INTERCEPTOR_H_

#include "base/memory/scoped_ptr.h"
#include "net/url_request/url_request_interceptor.h"

class GURL;

namespace net {
class URLRequest;
class URLRequestContextGetter;
class URLRequestJob;
class NetworkDelegate;
}

namespace android_webview {

class AwWebResourceResponse;

class AwRequestInterceptor : public net::URLRequestInterceptor {
 public:
  AwRequestInterceptor();
  virtual ~AwRequestInterceptor();

  
  virtual net::URLRequestJob* MaybeInterceptRequest(
      net::URLRequest* request,
      net::NetworkDelegate* network_delegate) const OVERRIDE;

 private:
  scoped_ptr<AwWebResourceResponse> QueryForAwWebResourceResponse(
      const GURL& location,
      net::URLRequest* request) const;

  DISALLOW_COPY_AND_ASSIGN(AwRequestInterceptor);
};

} 

#endif  