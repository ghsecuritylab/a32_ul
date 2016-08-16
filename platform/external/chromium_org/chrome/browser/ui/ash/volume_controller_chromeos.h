// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_VOLUME_CONTROLLER_CHROMEOS_H_
#define CHROME_BROWSER_UI_ASH_VOLUME_CONTROLLER_CHROMEOS_H_

#include "ash/volume_control_delegate.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chromeos/audio/cras_audio_handler.h"

class VolumeController : public ash::VolumeControlDelegate,
                         public chromeos::CrasAudioHandler::AudioObserver {
 public:
  VolumeController();
  virtual ~VolumeController();

  
  virtual bool HandleVolumeMute(const ui::Accelerator& accelerator) OVERRIDE;
  virtual bool HandleVolumeDown(const ui::Accelerator& accelerator) OVERRIDE;
  virtual bool HandleVolumeUp(const ui::Accelerator& accelerator) OVERRIDE;

  
  virtual void OnOutputVolumeChanged() OVERRIDE;
  virtual void OnOutputMuteChanged() OVERRIDE;

 private:

  DISALLOW_COPY_AND_ASSIGN(VolumeController);
};

#endif  