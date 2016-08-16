// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BROWSING_DATA_LOCAL_DATA_CONTAINER_H_
#define CHROME_BROWSER_BROWSING_DATA_LOCAL_DATA_CONTAINER_H_

#include <list>
#include <map>
#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/browsing_data/browsing_data_appcache_helper.h"
#include "chrome/browser/browsing_data/browsing_data_channel_id_helper.h"
#include "chrome/browser/browsing_data/browsing_data_cookie_helper.h"
#include "chrome/browser/browsing_data/browsing_data_database_helper.h"
#include "chrome/browser/browsing_data/browsing_data_file_system_helper.h"
#include "chrome/browser/browsing_data/browsing_data_indexed_db_helper.h"
#include "chrome/browser/browsing_data/browsing_data_local_storage_helper.h"
#include "chrome/browser/browsing_data/browsing_data_quota_helper.h"
#include "chrome/browser/browsing_data/browsing_data_service_worker_helper.h"
#include "net/ssl/channel_id_store.h"

class BrowsingDataFlashLSOHelper;
class CookiesTreeModel;
class LocalDataContainer;

namespace net {
class CanonicalCookie;
}

namespace {

typedef std::list<net::CanonicalCookie> CookieList;
typedef std::list<BrowsingDataDatabaseHelper::DatabaseInfo> DatabaseInfoList;
typedef std::list<BrowsingDataLocalStorageHelper::LocalStorageInfo>
    LocalStorageInfoList;
typedef std::list<BrowsingDataLocalStorageHelper::LocalStorageInfo>
    SessionStorageInfoList;
typedef std::list<content::IndexedDBInfo>
    IndexedDBInfoList;
typedef std::list<BrowsingDataFileSystemHelper::FileSystemInfo>
    FileSystemInfoList;
typedef std::list<BrowsingDataQuotaHelper::QuotaInfo> QuotaInfoList;
typedef net::ChannelIDStore::ChannelIDList ChannelIDList;
typedef std::list<content::ServiceWorkerUsageInfo> ServiceWorkerUsageInfoList;
typedef std::map<GURL, std::list<content::AppCacheInfo> > AppCacheInfoMap;
typedef std::vector<std::string> FlashLSODomainList;

}  

class LocalDataContainer {
 public:
  LocalDataContainer(
      BrowsingDataCookieHelper* cookie_helper,
      BrowsingDataDatabaseHelper* database_helper,
      BrowsingDataLocalStorageHelper* local_storage_helper,
      BrowsingDataLocalStorageHelper* session_storage_helper,
      BrowsingDataAppCacheHelper* appcache_helper,
      BrowsingDataIndexedDBHelper* indexed_db_helper,
      BrowsingDataFileSystemHelper* file_system_helper,
      BrowsingDataQuotaHelper* quota_helper,
      BrowsingDataChannelIDHelper* channel_id_helper,
      BrowsingDataServiceWorkerHelper* service_worker_helper,
      BrowsingDataFlashLSOHelper* flash_data_helper);
  virtual ~LocalDataContainer();

  
  
  void Init(CookiesTreeModel* delegate);

 private:
  friend class CookiesTreeModel;
  friend class CookieTreeAppCacheNode;
  friend class CookieTreeCookieNode;
  friend class CookieTreeDatabaseNode;
  friend class CookieTreeLocalStorageNode;
  friend class CookieTreeSessionStorageNode;
  friend class CookieTreeIndexedDBNode;
  friend class CookieTreeFileSystemNode;
  friend class CookieTreeQuotaNode;
  friend class CookieTreeChannelIDNode;
  friend class CookieTreeServiceWorkerNode;
  friend class CookieTreeFlashLSONode;

  
  void OnAppCacheModelInfoLoaded();
  void OnCookiesModelInfoLoaded(const net::CookieList& cookie_list);
  void OnDatabaseModelInfoLoaded(const DatabaseInfoList& database_info);
  void OnLocalStorageModelInfoLoaded(
      const LocalStorageInfoList& local_storage_info);
  void OnSessionStorageModelInfoLoaded(
      const LocalStorageInfoList& local_storage_info);
  void OnIndexedDBModelInfoLoaded(
      const IndexedDBInfoList& indexed_db_info);
  void OnFileSystemModelInfoLoaded(
      const FileSystemInfoList& file_system_info);
  void OnQuotaModelInfoLoaded(const QuotaInfoList& quota_info);
  void OnChannelIDModelInfoLoaded(const ChannelIDList& channel_id_list);
  void OnServiceWorkerModelInfoLoaded(
      const ServiceWorkerUsageInfoList& service_worker_info);
  void OnFlashLSOInfoLoaded(const FlashLSODomainList& domains);

  
  
  scoped_refptr<BrowsingDataAppCacheHelper> appcache_helper_;
  scoped_refptr<BrowsingDataCookieHelper> cookie_helper_;
  scoped_refptr<BrowsingDataDatabaseHelper> database_helper_;
  scoped_refptr<BrowsingDataLocalStorageHelper> local_storage_helper_;
  scoped_refptr<BrowsingDataLocalStorageHelper> session_storage_helper_;
  scoped_refptr<BrowsingDataIndexedDBHelper> indexed_db_helper_;
  scoped_refptr<BrowsingDataFileSystemHelper> file_system_helper_;
  scoped_refptr<BrowsingDataQuotaHelper> quota_helper_;
  scoped_refptr<BrowsingDataChannelIDHelper> channel_id_helper_;
  scoped_refptr<BrowsingDataServiceWorkerHelper> service_worker_helper_;
  scoped_refptr<BrowsingDataFlashLSOHelper> flash_lso_helper_;

  
  
  AppCacheInfoMap appcache_info_;
  CookieList cookie_list_;
  DatabaseInfoList database_info_list_;
  LocalStorageInfoList local_storage_info_list_;
  LocalStorageInfoList session_storage_info_list_;
  IndexedDBInfoList indexed_db_info_list_;
  FileSystemInfoList file_system_info_list_;
  QuotaInfoList quota_info_list_;
  ChannelIDList channel_id_list_;
  ServiceWorkerUsageInfoList service_worker_info_list_;
  FlashLSODomainList flash_lso_domain_list_;

  
  
  CookiesTreeModel* model_;

  base::WeakPtrFactory<LocalDataContainer> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(LocalDataContainer);
};

#endif  