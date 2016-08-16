// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_BLUETOOTH_BLUETOOTH_L2CAP_CHANNEL_MAC_H_
#define DEVICE_BLUETOOTH_BLUETOOTH_L2CAP_CHANNEL_MAC_H_

#import <IOBluetooth/IOBluetooth.h>
#import <IOKit/IOReturn.h>

#include "base/mac/scoped_nsobject.h"
#include "base/macros.h"
#include "base/memory/scoped_ptr.h"
#include "device/bluetooth/bluetooth_channel_mac.h"

@class BluetoothL2capChannelDelegate;

namespace device {

class BluetoothL2capChannelMac : public BluetoothChannelMac {
 public:
  
  
  
  BluetoothL2capChannelMac(BluetoothSocketMac* socket,
                           IOBluetoothL2CAPChannel* channel);
  virtual ~BluetoothL2capChannelMac();

  
  
  
  
  static scoped_ptr<BluetoothL2capChannelMac> OpenAsync(
      BluetoothSocketMac* socket,
      IOBluetoothDevice* device,
      BluetoothL2CAPPSM psm,
      IOReturn* status);

  
  virtual void SetSocket(BluetoothSocketMac* socket) OVERRIDE;
  virtual IOBluetoothDevice* GetDevice() OVERRIDE;
  virtual uint16_t GetOutgoingMTU() OVERRIDE;
  virtual IOReturn WriteAsync(void* data,
                              uint16_t length,
                              void* refcon) OVERRIDE;

  void OnChannelOpenComplete(IOBluetoothL2CAPChannel* channel,
                             IOReturn status);
  void OnChannelClosed(IOBluetoothL2CAPChannel* channel);
  void OnChannelDataReceived(IOBluetoothL2CAPChannel* channel,
                             void* data,
                             size_t length);
  void OnChannelWriteComplete(IOBluetoothL2CAPChannel* channel,
                              void* refcon,
                              IOReturn status);

 private:
  
  base::scoped_nsobject<IOBluetoothL2CAPChannel> channel_;

  
  base::scoped_nsobject<BluetoothL2capChannelDelegate> delegate_;

  DISALLOW_COPY_AND_ASSIGN(BluetoothL2capChannelMac);
};

}  

#endif  