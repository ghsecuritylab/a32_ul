// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_PULSE_PULSE_INPUT_H_
#define MEDIA_AUDIO_PULSE_PULSE_INPUT_H_

#include <pulse/pulseaudio.h>
#include <string>

#include "base/threading/thread_checker.h"
#include "media/audio/agc_audio_stream.h"
#include "media/audio/audio_device_name.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"
#include "media/base/audio_block_fifo.h"

namespace media {

class AudioManagerPulse;

class PulseAudioInputStream : public AgcAudioStream<AudioInputStream> {
 public:
  PulseAudioInputStream(AudioManagerPulse* audio_manager,
                        const std::string& device_name,
                        const AudioParameters& params,
                        pa_threaded_mainloop* mainloop,
                        pa_context* context);

  virtual ~PulseAudioInputStream();

  
  virtual bool Open() OVERRIDE;
  virtual void Start(AudioInputCallback* callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual double GetMaxVolume() OVERRIDE;
  virtual void SetVolume(double volume) OVERRIDE;
  virtual double GetVolume() OVERRIDE;
  virtual bool IsMuted() OVERRIDE;

 private:
  
  static void ReadCallback(pa_stream* handle, size_t length, void* user_data);
  static void StreamNotifyCallback(pa_stream* stream, void* user_data);
  static void VolumeCallback(pa_context* context, const pa_source_info* info,
                             int error, void* user_data);
  static void MuteCallback(pa_context* context,
                           const pa_source_info* info,
                           int error,
                           void* user_data);

  
  void ReadData();

  
  bool GetSourceInformation(pa_source_info_cb_t callback);

  AudioManagerPulse* audio_manager_;
  AudioInputCallback* callback_;
  std::string device_name_;
  AudioParameters params_;
  int channels_;
  double volume_;
  bool stream_started_;

  
  
  bool muted_;

  
  AudioBlockFifo fifo_;

  
  pa_threaded_mainloop* pa_mainloop_; 
  pa_context* pa_context_;  
  pa_stream* handle_;

  
  bool context_state_changed_;

  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(PulseAudioInputStream);
};

}  

#endif  