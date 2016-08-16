// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NETWORK_DELEGATE_H_
#define NET_BASE_NETWORK_DELEGATE_H_

#include <string>

#include "base/callback.h"
#include "base/strings/string16.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/auth.h"
#include "net/base/completion_callback.h"
#include "net/cookies/canonical_cookie.h"

class GURL;

namespace base {
class FilePath;
}

namespace net {

class CookieOptions;
class HttpRequestHeaders;
class HttpResponseHeaders;
class ProxyInfo;
class ProxyServer;
class ProxyService;
class SocketStream;
class URLRequest;

class NET_EXPORT NetworkDelegate : public base::NonThreadSafe {
 public:
  
  
  
  enum AuthRequiredResponse {
    AUTH_REQUIRED_RESPONSE_NO_ACTION,
    AUTH_REQUIRED_RESPONSE_SET_AUTH,
    AUTH_REQUIRED_RESPONSE_CANCEL_AUTH,
    AUTH_REQUIRED_RESPONSE_IO_PENDING,
  };
  typedef base::Callback<void(AuthRequiredResponse)> AuthCallback;

  virtual ~NetworkDelegate() {}

  
  
  
  
  int NotifyBeforeURLRequest(URLRequest* request,
                             const CompletionCallback& callback,
                             GURL* new_url);
  void NotifyResolveProxy(const GURL& url,
                          int load_flags,
                          const ProxyService& proxy_service,
                          ProxyInfo* result);
  void NotifyProxyFallback(const ProxyServer& bad_proxy,
                           int net_error);
  int NotifyBeforeSendHeaders(URLRequest* request,
                              const CompletionCallback& callback,
                              HttpRequestHeaders* headers);
  void NotifyBeforeSendProxyHeaders(URLRequest* request,
                                    const ProxyInfo& proxy_info,
                                    HttpRequestHeaders* headers);
  void NotifySendHeaders(URLRequest* request,
                         const HttpRequestHeaders& headers);
  int NotifyHeadersReceived(
      URLRequest* request,
      const CompletionCallback& callback,
      const HttpResponseHeaders* original_response_headers,
      scoped_refptr<HttpResponseHeaders>* override_response_headers,
      GURL* allowed_unsafe_redirect_url);
  void NotifyBeforeRedirect(URLRequest* request,
                            const GURL& new_location);
  void NotifyResponseStarted(URLRequest* request);
  void NotifyRawBytesRead(const URLRequest& request, int bytes_read);
  void NotifyCompleted(URLRequest* request, bool started);
  void NotifyURLRequestDestroyed(URLRequest* request);
  void NotifyPACScriptError(int line_number, const base::string16& error);
  AuthRequiredResponse NotifyAuthRequired(URLRequest* request,
                                          const AuthChallengeInfo& auth_info,
                                          const AuthCallback& callback,
                                          AuthCredentials* credentials);
  bool CanGetCookies(const URLRequest& request,
                     const CookieList& cookie_list);
  bool CanSetCookie(const URLRequest& request,
                    const std::string& cookie_line,
                    CookieOptions* options);
  bool CanAccessFile(const URLRequest& request,
                     const base::FilePath& path) const;
  bool CanThrottleRequest(const URLRequest& request) const;
  bool CanEnablePrivacyMode(const GURL& url,
                            const GURL& first_party_for_cookies) const;

  int NotifyBeforeSocketStreamConnect(SocketStream* socket,
                                      const CompletionCallback& callback);

  bool CancelURLRequestWithPolicyViolatingReferrerHeader(
      const URLRequest& request,
      const GURL& target_url,
      const GURL& referrer_url) const;

 private:
  
  
  

  
  
  
  
  
  
  
  
  
  
  
  
  virtual int OnBeforeURLRequest(URLRequest* request,
                                 const CompletionCallback& callback,
                                 GURL* new_url);

  
  
  
  virtual void OnResolveProxy(const GURL& url,
                              int load_flags,
                              const ProxyService& proxy_service,
                              ProxyInfo* result);

  
  
  
  
  virtual void OnProxyFallback(const ProxyServer& bad_proxy,
                               int net_error);

  
  
  
  
  
  virtual int OnBeforeSendHeaders(URLRequest* request,
                                  const CompletionCallback& callback,
                                  HttpRequestHeaders* headers);

  
  
  
  virtual void OnBeforeSendProxyHeaders(URLRequest* request,
                                        const ProxyInfo& proxy_info,
                                        HttpRequestHeaders* headers);

  
  
  
  virtual void OnSendHeaders(URLRequest* request,
                             const HttpRequestHeaders& headers);

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int OnHeadersReceived(
      URLRequest* request,
      const CompletionCallback& callback,
      const HttpResponseHeaders* original_response_headers,
      scoped_refptr<HttpResponseHeaders>* override_response_headers,
      GURL* allowed_unsafe_redirect_url);

  
  
  
  virtual void OnBeforeRedirect(URLRequest* request,
                                const GURL& new_location);

  
  virtual void OnResponseStarted(URLRequest* request);

  
  virtual void OnRawBytesRead(const URLRequest& request, int bytes_read);

  
  
  
  virtual void OnCompleted(URLRequest* request, bool started);

  
  
  
  virtual void OnURLRequestDestroyed(URLRequest* request);

  
  virtual void OnPACScriptError(int line_number,
                                const base::string16& error);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual AuthRequiredResponse OnAuthRequired(
      URLRequest* request,
      const AuthChallengeInfo& auth_info,
      const AuthCallback& callback,
      AuthCredentials* credentials);

  
  
  
  virtual bool OnCanGetCookies(const URLRequest& request,
                               const CookieList& cookie_list);

  
  
  
  virtual bool OnCanSetCookie(const URLRequest& request,
                              const std::string& cookie_line,
                              CookieOptions* options);

  
  
  
  virtual bool OnCanAccessFile(const URLRequest& request,
                               const base::FilePath& path) const;

  
  
  
  virtual bool OnCanThrottleRequest(const URLRequest& request) const;

  
  
  
  virtual bool OnCanEnablePrivacyMode(
      const GURL& url,
      const GURL& first_party_for_cookies) const;

  
  
  virtual int OnBeforeSocketStreamConnect(
      SocketStream* socket, const CompletionCallback& callback);

  
  
  
  
  
  virtual bool OnCancelURLRequestWithPolicyViolatingReferrerHeader(
      const URLRequest& request,
      const GURL& target_url,
      const GURL& referrer_url) const;
};

}  

#endif  