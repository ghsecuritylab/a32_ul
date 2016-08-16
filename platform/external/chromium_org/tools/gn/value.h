// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_VALUE_H_
#define TOOLS_GN_VALUE_H_

#include <map>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "tools/gn/err.h"

class ParseNode;
class Scope;

class Value {
 public:
  enum Type {
    NONE = 0,
    BOOLEAN,
    INTEGER,
    STRING,
    LIST,
    SCOPE,
  };

  Value();
  Value(const ParseNode* origin, Type t);
  Value(const ParseNode* origin, bool bool_val);
  Value(const ParseNode* origin, int64 int_val);
  Value(const ParseNode* origin, std::string str_val);
  Value(const ParseNode* origin, const char* str_val);
  
  
  
  
  
  Value(const ParseNode* origin, scoped_ptr<Scope> scope);

  Value(const Value& other);
  ~Value();

  Value& operator=(const Value& other);

  Type type() const { return type_; }

  
  static const char* DescribeType(Type t);

  
  const ParseNode* origin() const { return origin_; }
  void set_origin(const ParseNode* o) { origin_ = o; }

  bool& boolean_value() {
    DCHECK(type_ == BOOLEAN);
    return boolean_value_;
  }
  const bool& boolean_value() const {
    DCHECK(type_ == BOOLEAN);
    return boolean_value_;
  }

  int64& int_value() {
    DCHECK(type_ == INTEGER);
    return int_value_;
  }
  const int64& int_value() const {
    DCHECK(type_ == INTEGER);
    return int_value_;
  }

  std::string& string_value() {
    DCHECK(type_ == STRING);
    return string_value_;
  }
  const std::string& string_value() const {
    DCHECK(type_ == STRING);
    return string_value_;
  }

  std::vector<Value>& list_value() {
    DCHECK(type_ == LIST);
    return list_value_;
  }
  const std::vector<Value>& list_value() const {
    DCHECK(type_ == LIST);
    return list_value_;
  }

  Scope* scope_value() {
    DCHECK(type_ == SCOPE);
    return scope_value_.get();
  }
  const Scope* scope_value() const {
    DCHECK(type_ == SCOPE);
    return scope_value_.get();
  }
  void SetScopeValue(scoped_ptr<Scope> scope);

  
  
  
  std::string ToString(bool quote_strings) const;

  
  
  bool VerifyTypeIs(Type t, Err* err) const;

  
  bool operator==(const Value& other) const;
  bool operator!=(const Value& other) const;

 private:
  
  
  
  
  Type type_;
  std::string string_value_;
  bool boolean_value_;
  int64 int_value_;
  std::vector<Value> list_value_;
  scoped_ptr<Scope> scope_value_;

  const ParseNode* origin_;
};

#endif  