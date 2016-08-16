// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_TABS_TABS_WINDOWS_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_TABS_TABS_WINDOWS_API_H_

#include "base/memory/scoped_ptr.h"
#include "components/keyed_service/core/keyed_service.h"
#include "extensions/browser/browser_context_keyed_api_factory.h"
#include "extensions/browser/event_router.h"

namespace extensions {
class TabsEventRouter;
class WindowsEventRouter;

class TabsWindowsAPI : public BrowserContextKeyedAPI,
                       public EventRouter::Observer {
 public:
  explicit TabsWindowsAPI(content::BrowserContext* context);
  virtual ~TabsWindowsAPI();

  
  static TabsWindowsAPI* Get(content::BrowserContext* context);

  TabsEventRouter* tabs_event_router();
  WindowsEventRouter* windows_event_router();

  
  virtual void Shutdown() OVERRIDE;

  
  static BrowserContextKeyedAPIFactory<TabsWindowsAPI>* GetFactoryInstance();

  
  virtual void OnListenerAdded(const extensions::EventListenerInfo& details)
      OVERRIDE;

 private:
  friend class BrowserContextKeyedAPIFactory<TabsWindowsAPI>;

  content::BrowserContext* browser_context_;

  
  static const char* service_name() {
    return "TabsWindowsAPI";
  }
  static const bool kServiceIsNULLWhileTesting = true;

  scoped_ptr<TabsEventRouter> tabs_event_router_;
  scoped_ptr<WindowsEventRouter> windows_event_router_;
};

}  

#endif  