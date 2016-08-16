// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RLZ_WIN_LIB_RLZ_VALUE_STORE_REGISTRY_H_
#define RLZ_WIN_LIB_RLZ_VALUE_STORE_REGISTRY_H_

#include "base/compiler_specific.h"
#include "rlz/lib/rlz_value_store.h"

namespace rlz_lib {

class RlzValueStoreRegistry : public RlzValueStore {
 public:
  static std::wstring GetWideLibKeyName();

  virtual bool HasAccess(AccessType type) OVERRIDE;

  virtual bool WritePingTime(Product product, int64 time) OVERRIDE;
  virtual bool ReadPingTime(Product product, int64* time) OVERRIDE;
  virtual bool ClearPingTime(Product product) OVERRIDE;

  virtual bool WriteAccessPointRlz(AccessPoint access_point,
                                   const char* new_rlz) OVERRIDE;
  virtual bool ReadAccessPointRlz(AccessPoint access_point,
                                  char* rlz,
                                  size_t rlz_size) OVERRIDE;
  virtual bool ClearAccessPointRlz(AccessPoint access_point) OVERRIDE;

  virtual bool AddProductEvent(Product product, const char* event_rlz) OVERRIDE;
  virtual bool ReadProductEvents(Product product,
                                 std::vector<std::string>* events) OVERRIDE;
  virtual bool ClearProductEvent(Product product,
                                 const char* event_rlz) OVERRIDE;
  virtual bool ClearAllProductEvents(Product product) OVERRIDE;

  virtual bool AddStatefulEvent(Product product,
                                const char* event_rlz) OVERRIDE;
  virtual bool IsStatefulEvent(Product product,
                               const char* event_rlz) OVERRIDE;
  virtual bool ClearAllStatefulEvents(Product product) OVERRIDE;

  virtual void CollectGarbage() OVERRIDE;

 private:
  RlzValueStoreRegistry() {}
  DISALLOW_COPY_AND_ASSIGN(RlzValueStoreRegistry);
  friend class ScopedRlzValueStoreLock;
};

}  

#endif  
