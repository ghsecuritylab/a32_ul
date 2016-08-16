// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_TEST_MENU_RUNNER_TEST_API_H_
#define UI_VIEWS_TEST_MENU_RUNNER_TEST_API_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace views {

class MenuRunner;
class MenuRunnerHandler;

namespace test {

class MenuRunnerTestAPI {
 public:
  explicit MenuRunnerTestAPI(MenuRunner* menu_runner);
  ~MenuRunnerTestAPI();

  
  void SetMenuRunnerHandler(scoped_ptr<MenuRunnerHandler> menu_runner_handler);

 private:
  MenuRunner* menu_runner_;

  DISALLOW_COPY_AND_ASSIGN(MenuRunnerTestAPI);
};

}  

}  

#endif  