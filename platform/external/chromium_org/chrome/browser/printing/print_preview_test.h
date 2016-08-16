// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PRINTING_PRINT_PREVIEW_TEST_H_
#define CHROME_BROWSER_PRINTING_PRINT_PREVIEW_TEST_H_

#include "chrome/test/base/browser_with_test_window_test.h"

class PrintPreviewTest : public BrowserWithTestWindowTest {
 public:
  PrintPreviewTest();
  virtual ~PrintPreviewTest();

 protected:
  virtual void SetUp() OVERRIDE;

  
  virtual BrowserWindow* CreateBrowserWindow() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(PrintPreviewTest);
};

#endif  
