// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_POWER_VIDEO_ACTIVITY_NOTIFIER_H_
#define ASH_SYSTEM_CHROMEOS_POWER_VIDEO_ACTIVITY_NOTIFIER_H_

#include "ash/wm/video_detector.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/time/time.h"

namespace ash {

class VideoActivityNotifier : public VideoDetectorObserver,
                              public ShellObserver {
 public:
  explicit VideoActivityNotifier(VideoDetector* detector);
  virtual ~VideoActivityNotifier();

  
  virtual void OnVideoDetected(bool is_fullscreen) OVERRIDE;

  
  virtual void OnLockStateChanged(bool locked) OVERRIDE;

 private:
  VideoDetector* detector_;  

  
  base::TimeTicks last_notify_time_;

  
  bool screen_is_locked_;

  DISALLOW_COPY_AND_ASSIGN(VideoActivityNotifier);
};

}  

#endif  