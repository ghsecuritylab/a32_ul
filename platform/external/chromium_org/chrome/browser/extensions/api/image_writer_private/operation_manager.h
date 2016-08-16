// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_IMAGE_WRITER_PRIVATE_OPERATION_MANAGER_H_
#define CHROME_BROWSER_EXTENSIONS_API_IMAGE_WRITER_PRIVATE_OPERATION_MANAGER_H_

#include <map>
#include <string>
#include "base/bind.h"
#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observer.h"
#include "base/stl_util.h"
#include "base/timer/timer.h"
#include "chrome/browser/extensions/api/image_writer_private/image_writer_private_api.h"
#include "chrome/browser/extensions/api/image_writer_private/operation.h"
#include "chrome/common/extensions/api/image_writer_private.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/browser/browser_context_keyed_api_factory.h"
#include "extensions/browser/extension_registry_observer.h"
#include "url/gurl.h"

namespace image_writer_api = extensions::api::image_writer_private;

class Profile;

namespace content {
class BrowserContext;
}

namespace extensions {
class ExtensionRegistry;

namespace image_writer {

class Operation;

class OperationManager : public BrowserContextKeyedAPI,
                         public content::NotificationObserver,
                         public extensions::ExtensionRegistryObserver,
                         public base::SupportsWeakPtr<OperationManager> {
 public:
  typedef std::string ExtensionId;

  explicit OperationManager(content::BrowserContext* context);
  virtual ~OperationManager();

  virtual void Shutdown() OVERRIDE;

  
  void StartWriteFromUrl(const ExtensionId& extension_id,
                         GURL url,
                         const std::string& hash,
                         const std::string& device_path,
                         const Operation::StartWriteCallback& callback);

  
  void StartWriteFromFile(const ExtensionId& extension_id,
                          const base::FilePath& path,
                          const std::string& device_path,
                          const Operation::StartWriteCallback& callback);

  
  void CancelWrite(const ExtensionId& extension_id,
                   const Operation::CancelWriteCallback& callback);

  
  void DestroyPartitions(const ExtensionId& extension_id,
                         const std::string& device_path,
                         const Operation::StartWriteCallback& callback);

  
  virtual void OnProgress(const ExtensionId& extension_id,
                          image_writer_api::Stage stage,
                          int progress);
  
  virtual void OnComplete(const ExtensionId& extension_id);

  
  virtual void OnError(const ExtensionId& extension_id,
                       image_writer_api::Stage stage,
                       int progress,
                       const std::string& error_message);

  
  static BrowserContextKeyedAPIFactory<OperationManager>* GetFactoryInstance();
  static OperationManager* Get(content::BrowserContext* context);

 private:

  static const char* service_name() {
    return "OperationManager";
  }

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnExtensionUnloaded(
      content::BrowserContext* browser_context,
      const Extension* extension,
      UnloadedExtensionInfo::Reason reason) OVERRIDE;

  Operation* GetOperation(const ExtensionId& extension_id);
  void DeleteOperation(const ExtensionId& extension_id);

  friend class BrowserContextKeyedAPIFactory<OperationManager>;
  typedef std::map<ExtensionId, scoped_refptr<Operation> > OperationMap;

  content::BrowserContext* browser_context_;
  OperationMap operations_;
  content::NotificationRegistrar registrar_;

  
  ScopedObserver<ExtensionRegistry, ExtensionRegistryObserver>
      extension_registry_observer_;

  base::WeakPtrFactory<OperationManager> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(OperationManager);
};

}  
}  

#endif  