// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_NETWORK_DELEGATE_ERROR_OBSERVER_H_
#define NET_PROXY_NETWORK_DELEGATE_ERROR_OBSERVER_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "net/proxy/proxy_resolver_error_observer.h"

namespace base {
class MessageLoopProxy;
}

namespace net {

class NetworkDelegate;

class NET_EXPORT_PRIVATE NetworkDelegateErrorObserver
    : public ProxyResolverErrorObserver {
 public:
  NetworkDelegateErrorObserver(NetworkDelegate* network_delegate,
                               base::MessageLoopProxy* origin_loop);
  virtual ~NetworkDelegateErrorObserver();

  
  virtual void OnPACScriptError(int line_number, const base::string16& error)
      OVERRIDE;

 private:
  class Core;

  scoped_refptr<Core> core_;

  DISALLOW_COPY_AND_ASSIGN(NetworkDelegateErrorObserver);
};

}  

#endif  