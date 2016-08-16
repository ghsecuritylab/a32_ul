// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SHARED_WORKER_SHARED_WORKER_INSTANCE_H_
#define CONTENT_BROWSER_SHARED_WORKER_SHARED_WORKER_INSTANCE_H_

#include <string>

#include "base/basictypes.h"
#include "content/browser/shared_worker/worker_storage_partition.h"
#include "content/common/content_export.h"
#include "third_party/WebKit/public/web/WebContentSecurityPolicy.h"
#include "url/gurl.h"

namespace content {
class ResourceContext;

class CONTENT_EXPORT SharedWorkerInstance {
 public:
  SharedWorkerInstance(const GURL& url,
                       const base::string16& name,
                       const base::string16& content_security_policy,
                       blink::WebContentSecurityPolicyType security_policy_type,
                       ResourceContext* resource_context,
                       const WorkerStoragePartitionId& partition_id);
  SharedWorkerInstance(const SharedWorkerInstance& other);
  ~SharedWorkerInstance();

  
  
  
  
  
  
  bool Matches(const GURL& url,
               const base::string16& name,
               const WorkerStoragePartitionId& partition,
               ResourceContext* resource_context) const;
  bool Matches(const SharedWorkerInstance& other) const;

  
  const GURL& url() const { return url_; }
  const base::string16 name() const { return name_; }
  const base::string16 content_security_policy() const {
    return content_security_policy_;
  }
  blink::WebContentSecurityPolicyType security_policy_type() const {
    return security_policy_type_;
  }
  ResourceContext* resource_context() const {
    return resource_context_;
  }
  const WorkerStoragePartitionId& partition_id() const { return partition_id_; }

 private:
  const GURL url_;
  const base::string16 name_;
  const base::string16 content_security_policy_;
  const blink::WebContentSecurityPolicyType security_policy_type_;
  ResourceContext* const resource_context_;
  const WorkerStoragePartitionId partition_id_;
};

}  


#endif  