// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_LAYER_OWNER_DELEGATE_H_
#define UI_COMPOSITOR_LAYER_OWNER_DELEGATE_H_

#include "ui/compositor/compositor_export.h"

namespace ui {
class Layer;

class COMPOSITOR_EXPORT LayerOwnerDelegate {
 public:
  virtual void OnLayerRecreated(Layer* old_layer, Layer* new_layer) = 0;

 protected:
  virtual ~LayerOwnerDelegate() {}
};

}  

#endif  