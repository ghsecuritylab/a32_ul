// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_TEST_TEST_RENDER_VIEW_HOST_FACTORY_H_
#define CONTENT_TEST_TEST_RENDER_VIEW_HOST_FACTORY_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/browser/renderer_host/render_view_host_factory.h"

namespace content {

class SiteInstance;
class RenderViewHostDelegate;
class RenderProcessHostFactory;
class SessionStorageNamespace;

class TestRenderViewHostFactory : public RenderViewHostFactory {
 public:
  explicit TestRenderViewHostFactory(RenderProcessHostFactory* rph_factory);
  virtual ~TestRenderViewHostFactory();

  virtual void set_render_process_host_factory(
      RenderProcessHostFactory* rph_factory);
  virtual RenderViewHost* CreateRenderViewHost(
      SiteInstance* instance,
      RenderViewHostDelegate* delegate,
      RenderWidgetHostDelegate* widget_delegate,
      int routing_id,
      int main_frame_routing_id,
      bool swapped_out) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(TestRenderViewHostFactory);
};

}  

#endif  
