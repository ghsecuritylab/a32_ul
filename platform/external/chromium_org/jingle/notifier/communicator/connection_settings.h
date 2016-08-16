// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_NOTIFIER_COMMUNICATOR_CONNECTION_SETTINGS_H_
#define JINGLE_NOTIFIER_COMMUNICATOR_CONNECTION_SETTINGS_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "jingle/notifier/base/server_information.h"
#include "webrtc/base/socketaddress.h"

namespace buzz {
class XmppClientSettings;
}  

namespace notifier {

extern const uint16 kSslTcpPort;

enum SslTcpMode { DO_NOT_USE_SSLTCP, USE_SSLTCP };

struct ConnectionSettings {
 public:
  ConnectionSettings(const rtc::SocketAddress& server,
                     SslTcpMode ssltcp_mode,
                     SslTcpSupport ssltcp_support);
  ConnectionSettings();
  ~ConnectionSettings();

  bool Equals(const ConnectionSettings& settings) const;

  std::string ToString() const;

  
  void FillXmppClientSettings(buzz::XmppClientSettings* client_settings) const;

  rtc::SocketAddress server;
  SslTcpMode ssltcp_mode;
  SslTcpSupport ssltcp_support;
};

typedef std::vector<ConnectionSettings> ConnectionSettingsList;

ConnectionSettingsList MakeConnectionSettingsList(
    const ServerList& servers,
    bool try_ssltcp_first);

}  

#endif  