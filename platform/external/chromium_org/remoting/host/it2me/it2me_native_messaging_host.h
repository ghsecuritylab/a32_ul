// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_IT2ME_IT2ME_NATIVE_MESSAGING_HOST_H_
#define REMOTING_HOST_IT2ME_IT2ME_NATIVE_MESSAGING_HOST_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "extensions/browser/api/messaging/native_messaging_channel.h"
#include "remoting/base/auto_thread_task_runner.h"
#include "remoting/host/it2me/it2me_host.h"

namespace base {
class DictionaryValue;
}  

namespace remoting {

class It2MeNativeMessagingHost
    : public It2MeHost::Observer,
      public extensions::NativeMessagingChannel::EventHandler {
 public:
  It2MeNativeMessagingHost(
      scoped_refptr<AutoThreadTaskRunner> task_runner,
      scoped_ptr<extensions::NativeMessagingChannel> channel,
      scoped_ptr<It2MeHostFactory> factory);
  virtual ~It2MeNativeMessagingHost();

  void Start(const base::Closure& quit_closure);

  
  virtual void OnMessage(scoped_ptr<base::Value> message) OVERRIDE;
  virtual void OnDisconnect() OVERRIDE;

  
  virtual void OnClientAuthenticated(const std::string& client_username)
      OVERRIDE;
  virtual void OnStoreAccessCode(const std::string& access_code,
                                 base::TimeDelta access_code_lifetime) OVERRIDE;
  virtual void OnNatPolicyChanged(bool nat_traversal_enabled) OVERRIDE;
  virtual void OnStateChanged(It2MeHostState state) OVERRIDE;

  static std::string HostStateToString(It2MeHostState host_state);

 private:
  
  
  
  void ProcessHello(const base::DictionaryValue& message,
                    scoped_ptr<base::DictionaryValue> response) const;
  void ProcessConnect(const base::DictionaryValue& message,
                      scoped_ptr<base::DictionaryValue> response);
  void ProcessDisconnect(const base::DictionaryValue& message,
                         scoped_ptr<base::DictionaryValue> response);
  void SendErrorAndExit(scoped_ptr<base::DictionaryValue> response,
                        const std::string& description) const;

  base::Closure quit_closure_;

  scoped_refptr<AutoThreadTaskRunner> task_runner() const;

  scoped_ptr<extensions::NativeMessagingChannel> channel_;
  scoped_ptr<It2MeHostFactory> factory_;
  scoped_ptr<ChromotingHostContext> host_context_;
  scoped_refptr<It2MeHost> it2me_host_;

  
  It2MeHostState state_;
  std::string access_code_;
  base::TimeDelta access_code_lifetime_;
  std::string client_username_;

  
  XmppSignalStrategy::XmppServerConfig xmpp_server_config_;

  
  std::string directory_bot_jid_;

  base::WeakPtr<It2MeNativeMessagingHost> weak_ptr_;
  base::WeakPtrFactory<It2MeNativeMessagingHost> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(It2MeNativeMessagingHost);
};

}  

#endif  

