// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_WEBSTORE_DATA_FETCHER_DELEGATE_H_
#define CHROME_BROWSER_EXTENSIONS_WEBSTORE_DATA_FETCHER_DELEGATE_H_

#include <string>

#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
}

namespace extensions {

class WebstoreDataFetcherDelegate {
 public:
  
  virtual void OnWebstoreRequestFailure() = 0;

  
  
  virtual void OnWebstoreResponseParseSuccess(
      scoped_ptr<base::DictionaryValue> webstore_data) = 0;

  
  virtual void OnWebstoreResponseParseFailure(const std::string& error) = 0;

  
  static const char kAverageRatingKey[];
  static const char kIconUrlKey[];
  static const char kIdKey[];
  static const char kInlineInstallNotSupportedKey[];
  static const char kLocalizedDescriptionKey[];
  static const char kLocalizedNameKey[];
  static const char kManifestKey[];
  static const char kRatingCountKey[];
  static const char kRedirectUrlKey[];
  static const char kShowUserCountKey[];
  static const char kUsersKey[];
  static const char kVerifiedSiteKey[];
  static const char kVerifiedSitesKey[];

 protected:
  virtual ~WebstoreDataFetcherDelegate() {}
};

}  

#endif  