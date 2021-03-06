// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_AUDIO_SYNC_READER_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_AUDIO_SYNC_READER_H_

#include "base/process/process.h"
#include "base/sync_socket.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "media/audio/audio_output_controller.h"
#include "media/base/audio_bus.h"

#if defined(OS_POSIX)
#include "base/file_descriptor_posix.h"
#endif

namespace base {
class SharedMemory;
}

namespace content {

class AudioSyncReader : public media::AudioOutputController::SyncReader {
 public:
  AudioSyncReader(base::SharedMemory* shared_memory,
                  const media::AudioParameters& params);

  virtual ~AudioSyncReader();

  
  virtual void UpdatePendingBytes(uint32 bytes) OVERRIDE;
  virtual void Read(media::AudioBus* dest) OVERRIDE;
  virtual void Close() OVERRIDE;

  bool Init();
  bool PrepareForeignSocket(base::ProcessHandle process_handle,
                            base::SyncSocket::TransitDescriptor* descriptor);

 private:
  
  
  bool WaitUntilDataIsReady();

  const base::SharedMemory* const shared_memory_;

  
  
  const bool mute_audio_;

  
  scoped_ptr<base::CancelableSyncSocket> socket_;

  
  
  scoped_ptr<base::CancelableSyncSocket> foreign_socket_;

  
  scoped_ptr<media::AudioBus> output_bus_;

  
  const int packet_size_;

  
  
  size_t renderer_callback_count_;
  size_t renderer_missed_callback_count_;

  
  
  const base::TimeDelta maximum_wait_time_;

  
  
  uint32 buffer_index_;

  DISALLOW_COPY_AND_ASSIGN(AudioSyncReader);
};

}  

#endif  
