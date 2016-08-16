// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_MDNS_DNS_SD_DEVICE_LISTER_H_
#define CHROME_BROWSER_EXTENSIONS_API_MDNS_DNS_SD_DEVICE_LISTER_H_

#include <string>

#include "chrome/browser/extensions/api/mdns/dns_sd_delegate.h"
#include "chrome/browser/local_discovery/service_discovery_device_lister.h"

namespace local_discovery {
class ServiceDiscoveryClient;
}  

namespace extensions {

class DnsSdDeviceLister
    : public local_discovery::ServiceDiscoveryDeviceLister::Delegate {
 public:
  DnsSdDeviceLister(
      local_discovery::ServiceDiscoveryClient* service_discovery_client,
      DnsSdDelegate* delegate,
      const std::string& service_type);
  virtual ~DnsSdDeviceLister();

  virtual void Discover(bool force_update);

 protected:
  virtual void OnDeviceChanged(
      bool added,
      const local_discovery::ServiceDescription& service_description) OVERRIDE;
  virtual void OnDeviceRemoved(const std::string& service_name) OVERRIDE;
  virtual void OnDeviceCacheFlushed() OVERRIDE;

 private:
  
  DnsSdDelegate* const delegate_;
  local_discovery::ServiceDiscoveryDeviceLister device_lister_;
  bool started_;

  DISALLOW_COPY_AND_ASSIGN(DnsSdDeviceLister);
};

}  

#endif  