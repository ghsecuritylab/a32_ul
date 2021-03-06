// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_STREAM_PARSER_H_
#define MEDIA_BASE_STREAM_PARSER_H_

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "media/base/media_export.h"
#include "media/base/media_log.h"

namespace media {

class AudioDecoderConfig;
class StreamParserBuffer;
class TextTrackConfig;
class VideoDecoderConfig;

class MEDIA_EXPORT StreamParser {
 public:
  typedef std::deque<scoped_refptr<StreamParserBuffer> > BufferQueue;

  
  
  
  
  
  
  
  
  
  typedef int TrackId;

  
  typedef std::map<TrackId, TextTrackConfig> TextTrackConfigMap;

  
  typedef std::map<TrackId, const BufferQueue> TextBufferQueueMap;

  
  struct InitParameters {
    InitParameters(base::TimeDelta duration);

    
    base::TimeDelta duration;

    
    
    base::Time timeline_offset;

    
    
    bool auto_update_timestamp_offset;

    
    Demuxer::Liveness liveness;
  };

  
  
  
  typedef base::Callback<void(bool success,
                              const InitParameters& params)> InitCB;

  
  
  
  
  
  
  
  
  
  
  typedef base::Callback<bool(const AudioDecoderConfig&,
                              const VideoDecoderConfig&,
                              const TextTrackConfigMap&)> NewConfigCB;

  
  
  
  
  
  
  
  
  
  
  typedef base::Callback<bool(const BufferQueue&,
                              const BufferQueue&,
                              const TextBufferQueueMap&)> NewBuffersCB;

  
  typedef base::Callback<void()> NewMediaSegmentCB;

  
  
  
  
  typedef base::Callback<void(const std::string&,
                              const std::vector<uint8>&)> NeedKeyCB;

  StreamParser();
  virtual ~StreamParser();

  
  
  
  
  
  virtual void Init(const InitCB& init_cb,
                    const NewConfigCB& config_cb,
                    const NewBuffersCB& new_buffers_cb,
                    bool ignore_text_track,
                    const NeedKeyCB& need_key_cb,
                    const NewMediaSegmentCB& new_segment_cb,
                    const base::Closure& end_of_segment_cb,
                    const LogCB& log_cb) = 0;

  
  
  
  virtual void Flush() = 0;

  
  
  
  virtual bool Parse(const uint8* buf, int size) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(StreamParser);
};

MEDIA_EXPORT bool MergeBufferQueues(
    const StreamParser::BufferQueue& audio_buffers,
    const StreamParser::BufferQueue& video_buffers,
    const StreamParser::TextBufferQueueMap& text_buffers,
    StreamParser::BufferQueue* merged_buffers);

}  

#endif  
