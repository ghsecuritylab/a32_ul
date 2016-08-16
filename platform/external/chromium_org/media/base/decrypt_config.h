// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_DECRYPT_CONFIG_H_
#define MEDIA_BASE_DECRYPT_CONFIG_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "media/base/media_export.h"

namespace media {

struct SubsampleEntry {
  uint32 clear_bytes;
  uint32 cypher_bytes;
};

class MEDIA_EXPORT DecryptConfig {
 public:
  
  static const int kDecryptionKeySize = 16;

  
  
  
  
  
  
  
  DecryptConfig(const std::string& key_id,
                const std::string& iv,
                const std::vector<SubsampleEntry>& subsamples);
  ~DecryptConfig();

  const std::string& key_id() const { return key_id_; }
  const std::string& iv() const { return iv_; }
  const std::vector<SubsampleEntry>& subsamples() const { return subsamples_; }

 private:
  const std::string key_id_;

  
  const std::string iv_;

  
  
  const std::vector<SubsampleEntry> subsamples_;

  DISALLOW_COPY_AND_ASSIGN(DecryptConfig);
};

}  

#endif  