// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_SIGNALING_JINGLE_INFO_REQUEST_H_
#define REMOTING_SIGNALING_JINGLE_INFO_REQUEST_H_

#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/signaling/iq_sender.h"

namespace buzz {
class XmlElement;
}  

namespace rtc {
class SocketAddress;
}  

namespace remoting {

class SignalStrategy;

class JingleInfoRequest {
 public:
  
  
  typedef base::Callback<void(const std::string& relay_token,
                              const std::vector<std::string>& relay_servers,
                              const std::vector<rtc::SocketAddress>&
                                  stun_servers)> OnJingleInfoCallback;

  explicit JingleInfoRequest(SignalStrategy* signal_strategy);
  virtual ~JingleInfoRequest();

  void Send(const OnJingleInfoCallback& callback);

 private:
  struct PendingDnsRequest;

  void OnResponse(IqRequest* request, const buzz::XmlElement* stanza);

  IqSender iq_sender_;
  scoped_ptr<IqRequest> request_;
  OnJingleInfoCallback on_jingle_info_cb_;

  DISALLOW_COPY_AND_ASSIGN(JingleInfoRequest);
};

}  

#endif  