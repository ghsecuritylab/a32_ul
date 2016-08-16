// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_WEB_HISTORY_SERVICE_FACTORY_H_
#define CHROME_BROWSER_HISTORY_WEB_HISTORY_SERVICE_FACTORY_H_

#include "base/memory/singleton.h"
#include "chrome/browser/profiles/profile.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace history {
class WebHistoryService;
}

class WebHistoryServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  
  static WebHistoryServiceFactory* GetInstance();

  
  static history::WebHistoryService* GetForProfile(Profile* profile);

 protected:
  
  virtual KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<WebHistoryServiceFactory>;

  explicit WebHistoryServiceFactory();
  virtual ~WebHistoryServiceFactory();

  DISALLOW_COPY_AND_ASSIGN(WebHistoryServiceFactory);
};

#endif  