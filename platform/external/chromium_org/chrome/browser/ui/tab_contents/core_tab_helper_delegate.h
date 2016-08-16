// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TAB_CONTENTS_CORE_TAB_HELPER_DELEGATE_H_
#define CHROME_BROWSER_UI_TAB_CONTENTS_CORE_TAB_HELPER_DELEGATE_H_

#include "base/basictypes.h"

namespace content {
class WebContents;
}

class CoreTabHelperDelegate {
 public:
  
  
  
  virtual void SwapTabContents(content::WebContents* old_contents,
                               content::WebContents* new_contents,
                               bool did_start_load,
                               bool did_finish_load);

  
  
  virtual bool CanReloadContents(content::WebContents* web_contents) const;

  
  
  virtual bool CanSaveContents(content::WebContents* web_contents) const;

 protected:
  virtual ~CoreTabHelperDelegate();
};

#endif  