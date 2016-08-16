// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_INDEXED_DB_INDEXED_DB_CONNECTION_H_
#define CONTENT_BROWSER_INDEXED_DB_INDEXED_DB_CONNECTION_H_

#include "base/memory/ref_counted.h"
#include "content/browser/indexed_db/indexed_db_database.h"
#include "content/browser/indexed_db/indexed_db_database_callbacks.h"

namespace content {
class IndexedDBCallbacks;
class IndexedDBDatabaseError;

class CONTENT_EXPORT IndexedDBConnection {
 public:
  IndexedDBConnection(scoped_refptr<IndexedDBDatabase> db,
                      scoped_refptr<IndexedDBDatabaseCallbacks> callbacks);
  virtual ~IndexedDBConnection();

  
  virtual void ForceClose();
  virtual void Close();
  virtual bool IsConnected();

  void VersionChangeIgnored();

  IndexedDBDatabase* database() { return database_.get(); }
  IndexedDBDatabaseCallbacks* callbacks() { return callbacks_.get(); }

 private:
  
  scoped_refptr<IndexedDBDatabase> database_;

  
  
  scoped_refptr<IndexedDBDatabaseCallbacks> callbacks_;

  DISALLOW_COPY_AND_ASSIGN(IndexedDBConnection);
};

}  

#endif  