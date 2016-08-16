// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_BROWSER_PLUGIN_MOCK_BROWSER_PLUGIN_MANAGER_H_
#define CONTENT_RENDERER_BROWSER_PLUGIN_MOCK_BROWSER_PLUGIN_MANAGER_H_

#include "content/renderer/browser_plugin/browser_plugin_manager.h"

#include "base/memory/scoped_ptr.h"
#include "content/public/renderer/browser_plugin_delegate.h"
#include "ipc/ipc_message_utils.h"
#include "ipc/ipc_test_sink.h"

namespace content {

class MockBrowserPlugin;

class MockBrowserPluginManager : public BrowserPluginManager {
 public:
  MockBrowserPluginManager(RenderViewImpl* render_view);

  
  virtual BrowserPlugin* CreateBrowserPlugin(
      RenderViewImpl* render_view,
      blink::WebFrame* frame,
      scoped_ptr<BrowserPluginDelegate> delegate) OVERRIDE;

  
  IPC::TestSink& sink() { return sink_; }

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  MockBrowserPlugin* last_plugin() { return last_plugin_; }

 protected:
  virtual ~MockBrowserPluginManager();

  IPC::TestSink sink_;

  
  scoped_ptr<IPC::MessageReplyDeserializer> reply_deserializer_;

  MockBrowserPlugin* last_plugin_;

  DISALLOW_COPY_AND_ASSIGN(MockBrowserPluginManager);
};

}  

#endif  