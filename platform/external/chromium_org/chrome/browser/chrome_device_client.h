// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROME_DEVICE_CLIENT_H_
#define CHROME_BROWSER_CHROME_DEVICE_CLIENT_H_

#include "device/core/device_client.h"

#include "base/compiler_specific.h"
#include "base/macros.h"

class ChromeDeviceClient : device::DeviceClient {
 public:
  ChromeDeviceClient();
  virtual ~ChromeDeviceClient();

  
  virtual device::UsbService* GetUsbService() OVERRIDE;
  virtual device::HidService* GetHidService() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(ChromeDeviceClient);
};

#endif  