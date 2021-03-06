// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_SYNC_DRIVER_FAKE_GENERIC_CHANGE_PROCESSOR_H_
#define COMPONENTS_SYNC_DRIVER_FAKE_GENERIC_CHANGE_PROCESSOR_H_

#include "components/sync_driver/generic_change_processor.h"

#include "components/sync_driver/generic_change_processor_factory.h"
#include "components/sync_driver/sync_api_component_factory.h"
#include "sync/api/sync_error.h"
#include "sync/internal_api/public/base/model_type.h"

namespace sync_driver {

class FakeGenericChangeProcessor : public GenericChangeProcessor {
 public:
  FakeGenericChangeProcessor(syncer::ModelType type,
                             SyncApiComponentFactory* sync_factory);
  virtual ~FakeGenericChangeProcessor();

  
  void set_sync_model_has_user_created_nodes(bool has_nodes);
  void set_sync_model_has_user_created_nodes_success(bool success);

  
  virtual syncer::SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const syncer::SyncChangeList& change_list) OVERRIDE;
  virtual syncer::SyncError GetAllSyncDataReturnError(
      syncer::SyncDataList* data) const OVERRIDE;
  virtual bool GetDataTypeContext(std::string* context) const OVERRIDE;
  virtual int GetSyncCount() OVERRIDE;
  virtual bool SyncModelHasUserCreatedNodes(bool* has_nodes) OVERRIDE;
  virtual bool CryptoReadyIfNecessary() OVERRIDE;

 private:
  bool sync_model_has_user_created_nodes_;
  bool sync_model_has_user_created_nodes_success_;
};

class FakeGenericChangeProcessorFactory : public GenericChangeProcessorFactory {
 public:
  explicit FakeGenericChangeProcessorFactory(
      scoped_ptr<FakeGenericChangeProcessor> processor);
  virtual ~FakeGenericChangeProcessorFactory();
  virtual scoped_ptr<GenericChangeProcessor> CreateGenericChangeProcessor(
      syncer::ModelType type,
      syncer::UserShare* user_share,
      DataTypeErrorHandler* error_handler,
      const base::WeakPtr<syncer::SyncableService>& local_service,
      const base::WeakPtr<syncer::SyncMergeResult>& merge_result,
      SyncApiComponentFactory* sync_factory) OVERRIDE;

 private:
  scoped_ptr<FakeGenericChangeProcessor> processor_;
  DISALLOW_COPY_AND_ASSIGN(FakeGenericChangeProcessorFactory);
};

}  

#endif  
