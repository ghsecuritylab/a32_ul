// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_REPOST_FORM_WARNING_CONTROLLER_H_
#define CHROME_BROWSER_REPOST_FORM_WARNING_CONTROLLER_H_

#include "base/compiler_specific.h"
#include "chrome/browser/ui/tab_modal_confirm_dialog_delegate.h"
#include "content/public/browser/web_contents_observer.h"

class RepostFormWarningController : public TabModalConfirmDialogDelegate,
                                    public content::WebContentsObserver {
 public:
  explicit RepostFormWarningController(content::WebContents* web_contents);
  virtual ~RepostFormWarningController();

 private:
  
  virtual base::string16 GetTitle() OVERRIDE;
  virtual base::string16 GetDialogMessage() OVERRIDE;
  virtual base::string16 GetAcceptButtonTitle() OVERRIDE;
  virtual void OnAccepted() OVERRIDE;
  virtual void OnCanceled() OVERRIDE;
  virtual void OnClosed() OVERRIDE;

  
  virtual void BeforeFormRepostWarningShow() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(RepostFormWarningController);
};

#endif  