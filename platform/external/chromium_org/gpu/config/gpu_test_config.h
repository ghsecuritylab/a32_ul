// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_CONFIG_GPU_TEST_CONFIG_H_
#define GPU_CONFIG_GPU_TEST_CONFIG_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "gpu/gpu_export.h"

namespace gpu {

struct GPUInfo;

class GPU_EXPORT GPUTestConfig {
 public:
  enum OS {
    kOsUnknown = 0,
    kOsWinXP = 1 << 0,
    kOsWinVista = 1 << 1,
    kOsWin7 = 1 << 2,
    kOsWin8 = 1 << 3,
    kOsWin = kOsWinXP | kOsWinVista | kOsWin7 | kOsWin8,
    kOsMacLeopard = 1 << 4,
    kOsMacSnowLeopard = 1 << 5,
    kOsMacLion = 1 << 6,
    kOsMacMountainLion = 1 << 7,
    kOsMacMavericks = 1 << 8,
    kOsMac = kOsMacLeopard | kOsMacSnowLeopard | kOsMacLion |
             kOsMacMountainLion | kOsMacMavericks,
    kOsLinux = 1 << 9,
    kOsChromeOS = 1 << 10,
    kOsAndroid = 1 << 11,
  };

  enum BuildType {
    kBuildTypeUnknown = 0,
    kBuildTypeRelease = 1 << 0,
    kBuildTypeDebug = 1 << 1,
  };

  GPUTestConfig();
  virtual ~GPUTestConfig();

  void set_os(int32 os);
  void set_gpu_device_id(uint32 id);
  void set_build_type(int32 build_type);

  virtual void AddGPUVendor(uint32 gpu_vendor);

  int32 os() const { return os_; }
  const std::vector<uint32>& gpu_vendor() const { return gpu_vendor_; }
  uint32 gpu_device_id() const { return gpu_device_id_; }
  int32 build_type() const { return build_type_; }

  
  
  virtual bool IsValid() const;

  
  
  bool OverlapsWith(const GPUTestConfig& config) const;

  
  void DisableGPUInfoValidation();

 protected:
  void ClearGPUVendor();

  
  
  bool validate_gpu_info_;

 private:
  
  int32 os_;

  
  std::vector<uint32> gpu_vendor_;

  
  uint32 gpu_device_id_;

  
  int32 build_type_;
};

class GPU_EXPORT GPUTestBotConfig : public GPUTestConfig {
 public:
  GPUTestBotConfig() { }
  virtual ~GPUTestBotConfig();

  
  virtual void AddGPUVendor(uint32 gpu_vendor) OVERRIDE;

  
  bool SetGPUInfo(const GPUInfo& gpu_info);

  
  
  
  virtual bool IsValid() const OVERRIDE;

  
  
  bool Matches(const GPUTestConfig& config) const;
  bool Matches(const std::string& config_data) const;

  
  
  bool LoadCurrentConfig(const GPUInfo* gpu_info);

  
  static bool CurrentConfigMatches(const std::string& config_data);
  static bool CurrentConfigMatches(const std::vector<std::string>& configs);

  
  static bool GpuBlacklistedOnBot();
};

}  

#endif  

