// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_HANDSHAKE_STREAM_CREATE_HELPER_H_
#define NET_WEBSOCKETS_WEBSOCKET_HANDSHAKE_STREAM_CREATE_HELPER_H_

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/websockets/websocket_handshake_stream_base.h"
#include "net/websockets/websocket_stream.h"

namespace net {

class WebSocketBasicHandshakeStream;

class NET_EXPORT_PRIVATE WebSocketHandshakeStreamCreateHelper
    : public WebSocketHandshakeStreamBase::CreateHelper {
 public:
  
  explicit WebSocketHandshakeStreamCreateHelper(
      WebSocketStream::ConnectDelegate* connect_delegate,
      const std::vector<std::string>& requested_subprotocols);

  virtual ~WebSocketHandshakeStreamCreateHelper();

  

  
  virtual WebSocketHandshakeStreamBase* CreateBasicStream(
      scoped_ptr<ClientSocketHandle> connection,
      bool using_proxy) OVERRIDE;

  
  virtual WebSocketHandshakeStreamBase* CreateSpdyStream(
      const base::WeakPtr<SpdySession>& session,
      bool use_relative_url) OVERRIDE;

  
  
  scoped_ptr<WebSocketStream> Upgrade();

  
  
  
  
  void set_failure_message(std::string* failure_message) {
    failure_message_ = failure_message;
  }

 protected:
  
  
  virtual void OnStreamCreated(WebSocketBasicHandshakeStream* stream);

 private:
  const std::vector<std::string> requested_subprotocols_;

  
  
  
  
  
  WebSocketHandshakeStreamBase* stream_;

  WebSocketStream::ConnectDelegate* connect_delegate_;
  std::string* failure_message_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketHandshakeStreamCreateHelper);
};

}  

#endif  