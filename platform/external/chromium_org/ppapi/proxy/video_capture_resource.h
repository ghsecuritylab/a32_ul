// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_VIDEO_CAPTURE_RESOURCE_H_
#define PPAPI_PROXY_VIDEO_CAPTURE_RESOURCE_H_

#include "base/compiler_specific.h"
#include "ppapi/c/dev/ppp_video_capture_dev.h"
#include "ppapi/proxy/device_enumeration_resource_helper.h"
#include "ppapi/proxy/plugin_resource.h"
#include "ppapi/thunk/ppb_video_capture_api.h"

namespace ppapi {
namespace proxy {

class VideoCaptureResource
    : public PluginResource,
      public ::ppapi::thunk::PPB_VideoCapture_API {
 public:
  VideoCaptureResource(Connection connection,
                       PP_Instance instance,
                       PluginDispatcher* dispatcher);
  virtual ~VideoCaptureResource();

  
  virtual thunk::PPB_VideoCapture_API* AsPPB_VideoCapture_API() OVERRIDE {
    return this;
  }

  
  virtual int32_t EnumerateDevices(
      const PP_ArrayOutput& output,
      scoped_refptr<TrackedCallback> callback) OVERRIDE;
  virtual int32_t MonitorDeviceChange(
      PP_MonitorDeviceChangeCallback callback,
      void* user_data) OVERRIDE;
  virtual int32_t Open(const std::string& device_id,
                       const PP_VideoCaptureDeviceInfo_Dev& requested_info,
                       uint32_t buffer_count,
                       scoped_refptr<TrackedCallback> callback) OVERRIDE;
  virtual int32_t StartCapture() OVERRIDE;
  virtual int32_t ReuseBuffer(uint32_t buffer) OVERRIDE;
  virtual int32_t StopCapture() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual int32_t EnumerateDevicesSync(const PP_ArrayOutput& devices) OVERRIDE;

 protected:
  
  virtual void LastPluginRefWasDeleted() OVERRIDE;

 private:
  enum OpenState {
    BEFORE_OPEN,
    OPENED,
    CLOSED
  };

  
  virtual void OnReplyReceived(const ResourceMessageReplyParams& params,
                               const IPC::Message& msg) OVERRIDE;

  void OnPluginMsgOnDeviceInfo(const ResourceMessageReplyParams& params,
                               const struct PP_VideoCaptureDeviceInfo_Dev& info,
                               const std::vector<HostResource>& buffers,
                               uint32_t buffer_size);
  void OnPluginMsgOnStatus(const ResourceMessageReplyParams& params,
                           uint32_t status);
  void OnPluginMsgOnError(const ResourceMessageReplyParams& params,
                          uint32_t error);
  void OnPluginMsgOnBufferReady(const ResourceMessageReplyParams& params,
                                uint32_t buffer);

  void OnPluginMsgOpenReply(const ResourceMessageReplyParams& params);

  void SetBufferInUse(uint32_t buffer_index);

  
  const PPP_VideoCapture_Dev* ppp_video_capture_impl_;

  
  std::vector<bool> buffer_in_use_;

  
  scoped_refptr<TrackedCallback> open_callback_;
  OpenState open_state_;

  DeviceEnumerationResourceHelper enumeration_helper_;

  DISALLOW_COPY_AND_ASSIGN(VideoCaptureResource);
};

}  
}  

#endif  