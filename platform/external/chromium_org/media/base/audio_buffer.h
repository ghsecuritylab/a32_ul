// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_BUFFER_H_
#define MEDIA_BASE_AUDIO_BUFFER_H_

#include <vector>

#include "base/memory/aligned_memory.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "media/base/channel_layout.h"
#include "media/base/media_export.h"
#include "media/base/sample_format.h"

namespace media {
class AudioBus;

class MEDIA_EXPORT AudioBuffer
    : public base::RefCountedThreadSafe<AudioBuffer> {
 public:
  
  
  
  enum { kChannelAlignment = 32 };

  
  
  
  
  
  static scoped_refptr<AudioBuffer> CopyFrom(SampleFormat sample_format,
                                             ChannelLayout channel_layout,
                                             int channel_count,
                                             int sample_rate,
                                             int frame_count,
                                             const uint8* const* data,
                                             const base::TimeDelta timestamp);

  
  
  static scoped_refptr<AudioBuffer> CreateBuffer(SampleFormat sample_format,
                                                 ChannelLayout channel_layout,
                                                 int channel_count,
                                                 int sample_rate,
                                                 int frame_count);

  
  static scoped_refptr<AudioBuffer> CreateEmptyBuffer(
      ChannelLayout channel_layout,
      int channel_count,
      int sample_rate,
      int frame_count,
      const base::TimeDelta timestamp);

  
  
  
  static scoped_refptr<AudioBuffer> CreateEOSBuffer();

  
  
  
  
  
  void ReadFrames(int frames_to_copy,
                  int source_frame_offset,
                  int dest_frame_offset,
                  AudioBus* dest);

  
  
  
  
  void TrimStart(int frames_to_trim);

  
  
  void TrimEnd(int frames_to_trim);

  
  
  void TrimRange(int start, int end);

  
  int channel_count() const { return channel_count_; }

  
  int frame_count() const { return adjusted_frame_count_; }

  
  int sample_rate() const { return sample_rate_; }

  
  ChannelLayout channel_layout() const { return channel_layout_; }

  base::TimeDelta timestamp() const { return timestamp_; }
  base::TimeDelta duration() const { return duration_; }
  void set_timestamp(base::TimeDelta timestamp) { timestamp_ = timestamp; }

  
  bool end_of_stream() const { return end_of_stream_; }

  
  
  const std::vector<uint8*>& channel_data() const { return channel_data_; }

 private:
  friend class base::RefCountedThreadSafe<AudioBuffer>;

  
  
  
  
  
  AudioBuffer(SampleFormat sample_format,
              ChannelLayout channel_layout,
              int channel_count,
              int sample_rate,
              int frame_count,
              bool create_buffer,
              const uint8* const* data,
              const base::TimeDelta timestamp);

  virtual ~AudioBuffer();

  const SampleFormat sample_format_;
  const ChannelLayout channel_layout_;
  const int channel_count_;
  const int sample_rate_;
  int adjusted_frame_count_;
  int trim_start_;
  const bool end_of_stream_;
  base::TimeDelta timestamp_;
  base::TimeDelta duration_;

  
  scoped_ptr<uint8, base::AlignedFreeDeleter> data_;

  
  std::vector<uint8*> channel_data_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(AudioBuffer);
};

}  

#endif  
