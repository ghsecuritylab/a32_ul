// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_DEVTOOLS_AGENT_HOST_CLIENT_H_
#define CONTENT_PUBLIC_BROWSER_DEVTOOLS_AGENT_HOST_CLIENT_H_

#include <string>

#include "base/basictypes.h"
#include "content/common/content_export.h"

namespace content {

class DevToolsAgentHost;

class CONTENT_EXPORT DevToolsAgentHostClient {
 public:
  virtual ~DevToolsAgentHostClient() {}

  
  virtual void DispatchProtocolMessage(DevToolsAgentHost* agent_host,
                                       const std::string& message) = 0;

  
  virtual void AgentHostClosed(DevToolsAgentHost* agent_host,
                               bool replaced_with_another_client) = 0;
};

}  

#endif  