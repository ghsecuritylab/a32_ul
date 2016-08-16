// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_TEST_FAKE_SERVER_TOMBSTONE_ENTITY_H_
#define SYNC_TEST_FAKE_SERVER_TOMBSTONE_ENTITY_H_

#include <string>

#include "base/basictypes.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/protocol/sync.pb.h"
#include "sync/test/fake_server/fake_server_entity.h"

namespace fake_server {

class TombstoneEntity : public FakeServerEntity {
 public:
  virtual ~TombstoneEntity();

  
  static FakeServerEntity* Create(const std::string& id);

  
  virtual std::string GetParentId() const OVERRIDE;
  virtual void SerializeAsProto(sync_pb::SyncEntity* proto) OVERRIDE;
  virtual bool IsDeleted() const OVERRIDE;
  virtual bool IsFolder() const OVERRIDE;

 private:
  TombstoneEntity(const std::string& id, const syncer::ModelType& model_type);
};

}  

#endif  