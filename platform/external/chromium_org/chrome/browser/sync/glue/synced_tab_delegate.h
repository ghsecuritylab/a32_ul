// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_SYNCED_TAB_DELEGATE_H__
#define CHROME_BROWSER_SYNC_GLUE_SYNCED_TAB_DELEGATE_H__

#include <string>
#include <vector>

#include "components/sessions/session_id.h"

class Profile;

namespace content {
class NavigationEntry;
class WebContents;
}

namespace browser_sync {

class SyncedTabDelegate {
 public:
  virtual ~SyncedTabDelegate() {}

  

  virtual SessionID::id_type GetWindowId() const = 0;
  virtual SessionID::id_type GetSessionId() const = 0;
  virtual bool IsBeingDestroyed() const = 0;
  virtual Profile* profile() const = 0;

  

  virtual std::string GetExtensionAppId() const = 0;

  

  virtual int GetCurrentEntryIndex() const = 0;
  virtual int GetEntryCount() const = 0;
  virtual int GetPendingEntryIndex() const = 0;
  virtual content::NavigationEntry* GetPendingEntry() const = 0;
  virtual content::NavigationEntry* GetEntryAtIndex(int i) const = 0;
  virtual content::NavigationEntry* GetActiveEntry() const = 0;

  

  virtual bool ProfileIsSupervised() const = 0;
  virtual const std::vector<const content::NavigationEntry*>*
      GetBlockedNavigations() const = 0;

  virtual bool IsPinned() const = 0;
  virtual bool HasWebContents() const = 0;
  virtual content::WebContents* GetWebContents() const = 0;

  
  virtual int GetSyncId() const = 0;
  virtual void SetSyncId(int sync_id) = 0;
  
  static SyncedTabDelegate* ImplFromWebContents(
      content::WebContents* web_contents);
};

}  

#endif  
