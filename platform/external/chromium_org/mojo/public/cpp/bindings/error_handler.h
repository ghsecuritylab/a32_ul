// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_CPP_BINDINGS_ERROR_HANDLER_H_
#define MOJO_PUBLIC_CPP_BINDINGS_ERROR_HANDLER_H_

namespace mojo {

class ErrorHandler {
 public:
  virtual ~ErrorHandler() {}
  virtual void OnConnectionError() = 0;
};

}  

#endif  