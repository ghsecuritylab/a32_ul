// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_NET_BENCHMARKING_EXTENSION_H_
#define CHROME_RENDERER_NET_BENCHMARKING_EXTENSION_H_

#include "chrome/renderer/benchmarking_extension.h"

namespace extensions_v8 {

class NetBenchmarkingExtension {
 public:
  static v8::Extension* Get();
};

}  

#endif  