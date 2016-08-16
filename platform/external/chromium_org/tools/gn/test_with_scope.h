// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_TEST_WITH_SCOPE_H_
#define TOOLS_GN_TEST_WITH_SCOPE_H_

#include <vector>

#include "base/basictypes.h"
#include "tools/gn/build_settings.h"
#include "tools/gn/err.h"
#include "tools/gn/input_file.h"
#include "tools/gn/parse_tree.h"
#include "tools/gn/scope.h"
#include "tools/gn/settings.h"
#include "tools/gn/token.h"
#include "tools/gn/toolchain.h"
#include "tools/gn/value.h"

class TestWithScope {
 public:
  TestWithScope();
  ~TestWithScope();

  BuildSettings* build_settings() { return &build_settings_; }
  Settings* settings() { return &settings_; }
  Toolchain* toolchain() { return &toolchain_; }
  Scope* scope() { return &scope_; }

  
  
  
  std::string& print_output() { return print_output_; }

  
  
  
  
  static void SetupToolchain(Toolchain* toolchain);

 private:
  void AppendPrintOutput(const std::string& str);

  BuildSettings build_settings_;
  Settings settings_;
  Toolchain toolchain_;
  Scope scope_;

  std::string print_output_;

  DISALLOW_COPY_AND_ASSIGN(TestWithScope);
};

class TestParseInput {
 public:
  TestParseInput(const std::string& input);
  ~TestParseInput();

  
  bool has_error() const { return parse_err_.has_error(); }
  const Err& parse_err() const { return parse_err_; }

  const InputFile& input_file() const { return input_file_; }
  const std::vector<Token>& tokens() const { return tokens_; }
  const ParseNode* parsed() const { return parsed_.get(); }

 private:
  InputFile input_file_;

  std::vector<Token> tokens_;
  scoped_ptr<ParseNode> parsed_;

  Err parse_err_;

  DISALLOW_COPY_AND_ASSIGN(TestParseInput);
};

#endif  