// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_PERF_BROWSER_PERF_TEST_H_
#define CHROME_TEST_PERF_BROWSER_PERF_TEST_H_

#include "chrome/test/base/in_process_browser_test.h"

namespace base {
class CommandLine;
}

class BrowserPerfTest : public InProcessBrowserTest {
 public:
  BrowserPerfTest();
  virtual ~BrowserPerfTest();

  
  
  virtual void SetUpCommandLine(base::CommandLine* command_line) OVERRIDE;

  
  void PrintIOPerfInfo(const std::string& test_name);

  
  void PrintMemoryUsageInfo(const std::string& test_name);
};

#endif  