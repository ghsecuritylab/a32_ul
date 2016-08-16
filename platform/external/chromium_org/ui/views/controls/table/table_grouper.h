// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TABLE_TABLE_GROUPER_H_
#define UI_VIEWS_CONTROLS_TABLE_TABLE_GROUPER_H_

#include "ui/views/views_export.h"

namespace views {

struct VIEWS_EXPORT GroupRange {
  int start;
  int length;
};

class VIEWS_EXPORT TableGrouper {
 public:
  virtual void GetGroupRange(int model_index, GroupRange* range) = 0;

 protected:
  virtual ~TableGrouper() {}
};

}

#endif  