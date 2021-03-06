// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_TOKEN_CACHE_TOKEN_CACHE_SERVICE_H_
#define CHROME_BROWSER_EXTENSIONS_TOKEN_CACHE_TOKEN_CACHE_SERVICE_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/time/time.h"
#include "components/keyed_service/core/keyed_service.h"
#include "components/signin/core/browser/signin_manager_base.h"

class Profile;

namespace extensions {

class TokenCacheService : public KeyedService,
                          public SigninManagerBase::Observer {
 public:
  explicit TokenCacheService(Profile* profile);
  virtual ~TokenCacheService();

  
  
  
  
  void StoreToken(const std::string& token_name, const std::string& token_value,
                  base::TimeDelta time_to_live);

  
  
  std::string RetrieveToken(const std::string& token_name);

  
  virtual void Shutdown() OVERRIDE;

 private:
  friend class TokenCacheTest;
  FRIEND_TEST_ALL_PREFIXES(TokenCacheTest, SignoutTest);

  
  virtual void GoogleSignedOut(const std::string& account_id,
                               const std::string& username) OVERRIDE;

  struct TokenCacheData {
    std::string token;
    base::Time expiration_time;
  };

  
  std::map<std::string, TokenCacheData> token_cache_;
  const Profile* const profile_;

  DISALLOW_COPY_AND_ASSIGN(TokenCacheService);
};

}  

#endif  
