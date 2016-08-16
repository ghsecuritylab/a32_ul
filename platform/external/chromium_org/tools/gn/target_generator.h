// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_TARGET_GENERATOR_H_
#define TOOLS_GN_TARGET_GENERATOR_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "tools/gn/source_dir.h"
#include "tools/gn/target.h"

class BuildSettings;
class Err;
class FunctionCallNode;
class Location;
class Scope;
class Value;

class TargetGenerator {
 public:
  TargetGenerator(Target* target,
                  Scope* scope,
                  const FunctionCallNode* function_call,
                  Err* err);
  ~TargetGenerator();

  void Run();

  
  
  static void GenerateTarget(Scope* scope,
                             const FunctionCallNode* function_call,
                             const std::vector<Value>& args,
                             const std::string& output_type,
                             Err* err);

 protected:
  
  virtual void DoRun() = 0;

  const BuildSettings* GetBuildSettings() const;

  bool FillSources();
  bool FillPublic();
  bool FillInputs();
  bool FillConfigs();
  bool FillOutputs(bool allow_substitutions);

  
  
  
  bool EnsureSubstitutionIsInOutputDir(
      const SubstitutionPattern& pattern,
      const Value& original_value);

  Target* target_;
  Scope* scope_;
  const FunctionCallNode* function_call_;
  Err* err_;

 private:
  bool FillDependentConfigs();  
  bool FillData();
  bool FillDependencies();  
  bool FillTestonly();

  
  
  bool FillGenericConfigs(const char* var_name,
                          UniqueVector<LabelConfigPair>* dest);
  bool FillGenericDeps(const char* var_name, LabelTargetVector* dest);

  bool FillForwardDependentConfigs();

  DISALLOW_COPY_AND_ASSIGN(TargetGenerator);
};

#endif  