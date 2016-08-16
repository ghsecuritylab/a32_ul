// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_CONFIG_GPU_INFO_H_
#define GPU_CONFIG_GPU_INFO_H_


#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "base/version.h"
#include "build/build_config.h"
#include "gpu/config/dx_diag_node.h"
#include "gpu/config/gpu_performance_stats.h"
#include "gpu/gpu_export.h"
#include "media/video/video_encode_accelerator.h"

namespace gpu {

enum CollectInfoResult {
  kCollectInfoNone = 0,
  kCollectInfoSuccess = 1,
  kCollectInfoNonFatalFailure = 2,
  kCollectInfoFatalFailure = 3
};

struct GPU_EXPORT GPUInfo {
  struct GPU_EXPORT GPUDevice {
    GPUDevice();
    ~GPUDevice();

    
    uint32 vendor_id;

    
    
    uint32 device_id;

    
    
    bool active;

    
    
    
    
    std::string vendor_string;
    std::string device_string;
  };

  GPUInfo();
  ~GPUInfo();

  bool SupportsAccelerated2dCanvas() const {
    return !can_lose_context && !software_rendering;
  }

  
  
  base::TimeDelta initialization_time;

  
  bool optimus;

  
  bool amd_switchable;

  
  bool lenovo_dcute;

  
  
  Version display_link_version;

  
  GPUDevice gpu;

  
  std::vector<GPUDevice> secondary_gpus;

  
  
  
  
  
  uint64 adapter_luid;

  
  std::string driver_vendor;

  
  std::string driver_version;

  
  std::string driver_date;

  
  std::string pixel_shader_version;

  
  std::string vertex_shader_version;

  
  
  
  
  
  
  std::string machine_model_name;

  
  
  std::string machine_model_version;

  
  std::string gl_version;

  
  std::string gl_vendor;

  
  std::string gl_renderer;

  
  std::string gl_extensions;

  
  std::string gl_ws_vendor;

  
  std::string gl_ws_version;

  
  std::string gl_ws_extensions;

  
  
  uint32 gl_reset_notification_strategy;

  
  
  bool can_lose_context;

  
  GpuPerformanceStats performance_stats;

  bool software_rendering;

  
  
  bool direct_rendering;

  
  bool sandboxed;

  
  int process_crash_count;

  
  
  CollectInfoResult basic_info_state;
  CollectInfoResult context_info_state;
#if defined(OS_WIN)
  CollectInfoResult dx_diagnostics_info_state;

  
  DxDiagNode dx_diagnostics;
#endif

  std::vector<media::VideoEncodeAccelerator::SupportedProfile>
      video_encode_accelerator_supported_profiles;
  
  

  
  
  
  
  class Enumerator {
   public:
    
    
    
    
    virtual void AddInt64(const char* name, int64 value) = 0;
    virtual void AddInt(const char* name, int value) = 0;
    virtual void AddString(const char* name, const std::string& value) = 0;
    virtual void AddBool(const char* name, bool value) = 0;
    virtual void AddTimeDeltaInSecondsF(const char* name,
                                        const base::TimeDelta& value) = 0;

    
    virtual void BeginGPUDevice() = 0;
    virtual void EndGPUDevice() = 0;

    
    
    virtual void BeginVideoEncodeAcceleratorSupportedProfile() = 0;
    virtual void EndVideoEncodeAcceleratorSupportedProfile() = 0;

    
    
    virtual void BeginAuxAttributes() = 0;
    virtual void EndAuxAttributes() = 0;

   protected:
    virtual ~Enumerator() {}
  };

  
  void EnumerateFields(Enumerator* enumerator) const;
};

}  

#endif  