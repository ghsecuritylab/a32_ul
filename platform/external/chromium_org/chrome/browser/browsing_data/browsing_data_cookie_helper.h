// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_COOKIE_HELPER_H_
#define CHROME_BROWSER_BROWSING_DATA_BROWSING_DATA_COOKIE_HELPER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/browsing_data/canonical_cookie_hash.h"
#include "net/cookies/cookie_monster.h"

class GURL;

namespace net {
class CanonicalCookie;
class URLRequestContextGetter;
}

class BrowsingDataCookieHelper
    : public base::RefCountedThreadSafe<BrowsingDataCookieHelper> {
 public:
  explicit BrowsingDataCookieHelper(
      net::URLRequestContextGetter* request_context_getter);

  
  
  
  virtual void StartFetching(
      const base::Callback<void(const net::CookieList& cookies)>& callback);

  
  
  virtual void DeleteCookie(const net::CanonicalCookie& cookie);

 protected:
  friend class base::RefCountedThreadSafe<BrowsingDataCookieHelper>;
  virtual ~BrowsingDataCookieHelper();

  net::URLRequestContextGetter* request_context_getter() {
    return request_context_getter_.get();
  }

 private:
  
  void FetchCookiesOnIOThread();

  
  void OnFetchComplete(const net::CookieList& cookies);

  
  void NotifyInUIThread(const net::CookieList& cookies);

  
  void DeleteCookieOnIOThread(const net::CanonicalCookie& cookie);

  
  
  
  
  bool is_fetching_;

  scoped_refptr<net::URLRequestContextGetter> request_context_getter_;

  
  base::Callback<void(const net::CookieList& cookies)> completion_callback_;

  DISALLOW_COPY_AND_ASSIGN(BrowsingDataCookieHelper);
};

class CannedBrowsingDataCookieHelper : public BrowsingDataCookieHelper {
 public:
  typedef std::map<GURL, canonical_cookie::CookieHashSet*> OriginCookieSetMap;

  explicit CannedBrowsingDataCookieHelper(
      net::URLRequestContextGetter* request_context);

  
  
  
  void AddReadCookies(const GURL& frame_url,
                      const GURL& request_url,
                      const net::CookieList& cookie_list);

  
  
  
  
  
  
  
  
  
  
  
  void AddChangedCookie(const GURL& frame_url,
                        const GURL& request_url,
                        const std::string& cookie_line,
                        const net::CookieOptions& options);

  
  void Reset();

  
  bool empty() const;

  
  virtual void StartFetching(
      const net::CookieMonster::GetCookieListCallback& callback) OVERRIDE;
  virtual void DeleteCookie(const net::CanonicalCookie& cookie) OVERRIDE;

  
  size_t GetCookieCount() const;

  
  const OriginCookieSetMap& origin_cookie_set_map() {
    return origin_cookie_set_map_;
  }

 private:
  
  
  
  bool DeleteMatchingCookie(const net::CanonicalCookie& add_cookie,
                            canonical_cookie::CookieHashSet* cookie_set);

  virtual ~CannedBrowsingDataCookieHelper();

  
  canonical_cookie::CookieHashSet* GetCookiesFor(const GURL& origin);

  
  void AddCookie(const GURL& frame_url,
                 const net::CanonicalCookie& cookie);

  
  OriginCookieSetMap origin_cookie_set_map_;

  DISALLOW_COPY_AND_ASSIGN(CannedBrowsingDataCookieHelper);
};

#endif  