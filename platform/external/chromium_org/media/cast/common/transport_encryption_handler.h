// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_COMMON_TRANSPORT_ENCRYPTION_HANDLER_H_
#define MEDIA_CAST_COMMON_TRANSPORT_ENCRYPTION_HANDLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "base/threading/non_thread_safe.h"

namespace crypto {
class Encryptor;
class SymmetricKey;
}

namespace media {
namespace cast {

class TransportEncryptionHandler : public base::NonThreadSafe {
 public:
  TransportEncryptionHandler();
  ~TransportEncryptionHandler();

  bool Initialize(std::string aes_key, std::string aes_iv_mask);

  bool Encrypt(uint32 frame_id,
               const base::StringPiece& data,
               std::string* encrypted_data);

  bool Decrypt(uint32 frame_id,
               const base::StringPiece& ciphertext,
               std::string* plaintext);

  bool is_activated() const { return is_activated_; }

 private:
  scoped_ptr<crypto::SymmetricKey> key_;
  scoped_ptr<crypto::Encryptor> encryptor_;
  std::string iv_mask_;
  bool is_activated_;

  DISALLOW_COPY_AND_ASSIGN(TransportEncryptionHandler);
};

}  
}  

#endif  