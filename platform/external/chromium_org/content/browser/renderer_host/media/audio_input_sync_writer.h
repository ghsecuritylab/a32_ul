// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_AUDIO_INPUT_SYNC_WRITER_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_AUDIO_INPUT_SYNC_WRITER_H_

#include "base/memory/scoped_vector.h"
#include "base/process/process.h"
#include "base/sync_socket.h"
#include "base/time/time.h"
#include "media/audio/audio_input_controller.h"
#include "media/audio/audio_parameters.h"
#include "media/base/audio_bus.h"

#if defined(OS_POSIX)
#include "base/file_descriptor_posix.h"
#endif

namespace base {
class SharedMemory;
}

namespace content {
class AudioInputSyncWriter : public media::AudioInputController::SyncWriter {
 public:
  explicit AudioInputSyncWriter(base::SharedMemory* shared_memory,
                                int shared_memory_segment_count,
                                const media::AudioParameters& params);

  virtual ~AudioInputSyncWriter();

  
  virtual void UpdateRecordedBytes(uint32 bytes) OVERRIDE;
  virtual void Write(const media::AudioBus* data,
                     double volume,
                     bool key_pressed) OVERRIDE;
  virtual void Close() OVERRIDE;

  bool Init();
  bool PrepareForeignSocket(base::ProcessHandle process_handle,
                            base::SyncSocket::TransitDescriptor* descriptor);

 private:
  base::SharedMemory* shared_memory_;
  uint32 shared_memory_segment_size_;
  uint32 shared_memory_segment_count_;
  uint32 current_segment_id_;

  
  scoped_ptr<base::CancelableSyncSocket> socket_;

  
  
  scoped_ptr<base::CancelableSyncSocket> foreign_socket_;

  
  base::Time creation_time_;

  
  base::Time last_write_time_;

  
  const int audio_bus_memory_size_;

  
  
  ScopedVector<media::AudioBus> audio_buses_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(AudioInputSyncWriter);
};

}  

#endif  