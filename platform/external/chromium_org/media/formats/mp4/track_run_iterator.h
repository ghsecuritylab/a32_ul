// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FORMATS_MP4_TRACK_RUN_ITERATOR_H_
#define MEDIA_FORMATS_MP4_TRACK_RUN_ITERATOR_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "media/base/media_export.h"
#include "media/base/media_log.h"
#include "media/base/stream_parser_buffer.h"
#include "media/formats/mp4/box_definitions.h"
#include "media/formats/mp4/cenc.h"

namespace media {

class DecryptConfig;

namespace mp4 {

base::TimeDelta MEDIA_EXPORT TimeDeltaFromRational(int64 numer, int64 denom);
DecodeTimestamp MEDIA_EXPORT DecodeTimestampFromRational(int64 numer,
                                                         int64 denom);

struct SampleInfo;
struct TrackRunInfo;

class MEDIA_EXPORT TrackRunIterator {
 public:
  
  
  TrackRunIterator(const Movie* moov, const LogCB& log_cb);
  ~TrackRunIterator();

  
  bool Init(const MovieFragment& moof);

  
  bool IsRunValid() const;
  bool IsSampleValid() const;

  
  
  void AdvanceRun();
  void AdvanceSample();

  
  
  bool AuxInfoNeedsToBeCached();

  
  
  
  bool CacheAuxInfo(const uint8* buf, int size);

  
  
  
  
  
  int64 GetMaxClearOffset();

  
  uint32 track_id() const;
  int64 aux_info_offset() const;
  int aux_info_size() const;
  bool is_encrypted() const;
  bool is_audio() const;
  
  const AudioSampleEntry& audio_description() const;
  const VideoSampleEntry& video_description() const;

  
  int64 sample_offset() const;
  int sample_size() const;
  DecodeTimestamp dts() const;
  base::TimeDelta cts() const;
  base::TimeDelta duration() const;
  bool is_keyframe() const;
  bool is_random_access_point() const;

  
  
  scoped_ptr<DecryptConfig> GetDecryptConfig();

 private:
  void ResetRun();
  const TrackEncryption& track_encryption() const;

  uint32 GetGroupDescriptionIndex(uint32 sample_index) const;
  const CencSampleEncryptionInfoEntry& GetSampleEncryptionInfoEntry(
      uint32 group_description_index) const;

  
  bool IsSampleEncrypted(size_t sample_index) const;
  uint8 GetIvSize(size_t sample_index) const;
  const std::vector<uint8>& GetKeyId(size_t sample_index) const;

  const Movie* moov_;
  LogCB log_cb_;

  std::vector<TrackRunInfo> runs_;
  std::vector<TrackRunInfo>::const_iterator run_itr_;
  std::vector<SampleInfo>::const_iterator sample_itr_;

  std::vector<FrameCENCInfo> cenc_info_;

  int64 sample_dts_;
  int64 sample_offset_;

  DISALLOW_COPY_AND_ASSIGN(TrackRunIterator);
};

}  
}  

#endif  
