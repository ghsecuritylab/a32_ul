// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_NETWORK_MONITOR_HOST_H_
#define CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_NETWORK_MONITOR_HOST_H_

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "content/common/content_export.h"
#include "net/base/net_util.h"
#include "net/base/network_change_notifier.h"
#include "ppapi/host/host_message_context.h"
#include "ppapi/host/resource_host.h"

namespace content {

class BrowserPpapiHostImpl;

class CONTENT_EXPORT PepperNetworkMonitorHost
    : public ppapi::host::ResourceHost,
      public net::NetworkChangeNotifier::IPAddressObserver {
 public:
  PepperNetworkMonitorHost(BrowserPpapiHostImpl* host,
                           PP_Instance instance,
                           PP_Resource resource);

  virtual ~PepperNetworkMonitorHost();

  
  virtual void OnIPAddressChanged() OVERRIDE;

 private:
  void OnPermissionCheckResult(bool can_use_network_monitor);

  void GetAndSendNetworkList();
  void SendNetworkList(scoped_ptr<net::NetworkInterfaceList> list);

  ppapi::host::ReplyMessageContext reply_context_;

  base::WeakPtrFactory<PepperNetworkMonitorHost> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PepperNetworkMonitorHost);
};

}  

#endif  