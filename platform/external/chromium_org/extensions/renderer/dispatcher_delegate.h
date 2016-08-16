// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_RENDERER_DISPATCHER_DELEGATE_H_
#define EXTENSIONS_RENDERER_DISPATCHER_DELEGATE_H_

#include <set>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "extensions/common/features/feature.h"
#include "v8/include/v8.h"

namespace blink {
class WebFrame;
}

namespace extensions {
class Dispatcher;
class Extension;
class ModuleSystem;
class ResourceBundleSourceMap;
class ScriptContext;
class URLPatternSet;

class DispatcherDelegate {
 public:
  virtual ~DispatcherDelegate() {}

  
  virtual scoped_ptr<ScriptContext> CreateScriptContext(
      const v8::Handle<v8::Context>& v8_context,
      blink::WebFrame* frame,
      const Extension* extension,
      Feature::Context context_type,
      const Extension* effective_extension,
      Feature::Context effective_context_type) = 0;

  
  virtual void InitOriginPermissions(const Extension* extension,
                                     bool is_extension_active) {}

  
  virtual void RegisterNativeHandlers(Dispatcher* dispatcher,
                                      ModuleSystem* module_system,
                                      ScriptContext* context) {}

  
  virtual void PopulateSourceMap(ResourceBundleSourceMap* source_map) {}

  
  virtual void RequireAdditionalModules(ScriptContext* context,
                                        bool is_within_platform_app) {}

  
  
  virtual void OnActiveExtensionsUpdated(
      const std::set<std::string>& extension_ids) {}

  
  
  
  virtual void SetChannel(int channel) {}

  
  
  
  virtual void ClearTabSpecificPermissions(
      const extensions::Dispatcher* dispatcher,
      int tab_id,
      const std::vector<std::string>& extension_ids) {}

  
  
  
  virtual void UpdateTabSpecificPermissions(
      const extensions::Dispatcher* dispatcher,
      const GURL& url,
      int tab_id,
      const std::string& extension_id,
      const extensions::URLPatternSet& origin_set) {}
};

}  

#endif  