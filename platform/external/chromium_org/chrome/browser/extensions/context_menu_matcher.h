// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_CONTEXT_MENU_MATCHER_H_
#define CHROME_BROWSER_EXTENSIONS_CONTEXT_MENU_MATCHER_H_

#include <map>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "chrome/browser/extensions/menu_manager.h"
#include "ui/base/models/simple_menu_model.h"

class ExtensionContextMenuBrowserTest;

namespace content {
class BrowserContext;
}

namespace extensions {

class ContextMenuMatcher {
 public:
  static const size_t kMaxExtensionItemTitleLength;

  
  
  static int ConvertToExtensionsCustomCommandId(int id);

  
  static bool IsExtensionsCustomCommandId(int id);

  
  
  ContextMenuMatcher(content::BrowserContext* context,
                     ui::SimpleMenuModel::Delegate* delegate,
                     ui::SimpleMenuModel* menu_model,
                     const base::Callback<bool(const MenuItem*)>& filter);

  
  
  
  
  
  void AppendExtensionItems(const MenuItem::ExtensionKey& extension_key,
                            const base::string16& selection_text,
                            int* index,
                            bool is_action_menu);

  void Clear();

  
  
  base::string16 GetTopLevelContextMenuTitle(
      const MenuItem::ExtensionKey& extension_key,
      const base::string16& selection_text);

  bool IsCommandIdChecked(int command_id) const;
  bool IsCommandIdEnabled(int command_id) const;
  void ExecuteCommand(int command_id,
                      content::WebContents* web_contents,
                      const content::ContextMenuParams& params);

 private:
  friend class ::ExtensionContextMenuBrowserTest;

  bool GetRelevantExtensionTopLevelItems(
      const MenuItem::ExtensionKey& extension_key,
      const Extension** extension,
      bool* can_cross_incognito,
      MenuItem::List* items);

  MenuItem::List GetRelevantExtensionItems(
      const MenuItem::List& items,
      bool can_cross_incognito);

  
  void RecursivelyAppendExtensionItems(const MenuItem::List& items,
                                       bool can_cross_incognito,
                                       const base::string16& selection_text,
                                       ui::SimpleMenuModel* menu_model,
                                       int* index,
                                       bool is_action_menu_top_level);

  
  extensions::MenuItem* GetExtensionMenuItem(int id) const;

  
  void SetExtensionIcon(const std::string& extension_id);

  content::BrowserContext* browser_context_;
  ui::SimpleMenuModel* menu_model_;
  ui::SimpleMenuModel::Delegate* delegate_;

  base::Callback<bool(const MenuItem*)> filter_;

  
  std::map<int, extensions::MenuItem::Id> extension_item_map_;

  
  ScopedVector<ui::SimpleMenuModel> extension_menu_models_;

  DISALLOW_COPY_AND_ASSIGN(ContextMenuMatcher);
};

}  

#endif  