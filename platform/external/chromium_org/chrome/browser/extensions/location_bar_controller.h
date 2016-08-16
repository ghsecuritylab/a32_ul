// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_LOCATION_BAR_CONTROLLER_H_
#define CHROME_BROWSER_EXTENSIONS_LOCATION_BAR_CONTROLLER_H_

#include <map>
#include <vector>

#include "base/macros.h"
#include "base/memory/linked_ptr.h"
#include "base/scoped_observer.h"
#include "extensions/browser/extension_registry_observer.h"

class ExtensionAction;

namespace content {
class WebContents;
class BrowserContext;
}

namespace extensions {
class Extension;
class ExtensionActionManager;
class ExtensionRegistry;

class LocationBarController : public ExtensionRegistryObserver {
 public:
  explicit LocationBarController(content::WebContents* web_contents);
  virtual ~LocationBarController();

  
  std::vector<ExtensionAction*> GetCurrentActions();

 private:
  
  virtual void OnExtensionLoaded(
      content::BrowserContext* browser_context,
      const Extension* extension) OVERRIDE;
  virtual void OnExtensionUnloaded(
      content::BrowserContext* browser_context,
      const Extension* extension,
      UnloadedExtensionInfo::Reason reason) OVERRIDE;

  
  content::WebContents* web_contents_;

  
  content::BrowserContext* browser_context_;

  
  ExtensionActionManager* action_manager_;

  
  
  bool should_show_page_actions_;

  
  
  typedef std::map<std::string, linked_ptr<ExtensionAction> >
      ExtensionActionMap;
  ExtensionActionMap active_script_actions_;

  ScopedObserver<ExtensionRegistry, ExtensionRegistryObserver>
      extension_registry_observer_;

  DISALLOW_COPY_AND_ASSIGN(LocationBarController);
};

}  

#endif  