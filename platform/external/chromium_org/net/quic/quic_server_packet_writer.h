// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_SERVER_PACKET_WRITER_H_
#define NET_QUIC_QUIC_SERVER_PACKET_WRITER_H_

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "net/base/ip_endpoint.h"
#include "net/quic/quic_connection.h"
#include "net/quic/quic_packet_writer.h"
#include "net/quic/quic_protocol.h"

namespace net {

class QuicBlockedWriterInterface;
class UDPServerSocket;
struct WriteResult;

class QuicServerPacketWriter : public QuicPacketWriter {
 public:
  typedef base::Callback<void(WriteResult)> WriteCallback;

  QuicServerPacketWriter(UDPServerSocket* socket,
                         QuicBlockedWriterInterface* blocked_writer);
  virtual ~QuicServerPacketWriter();

  
  
  
  virtual WriteResult WritePacketWithCallback(
      const char* buffer,
      size_t buf_len,
      const IPAddressNumber& self_address,
      const IPEndPoint& peer_address,
      WriteCallback callback);

  void OnWriteComplete(int rv);

  
  virtual bool IsWriteBlockedDataBuffered() const OVERRIDE;
  virtual bool IsWriteBlocked() const OVERRIDE;
  virtual void SetWritable() OVERRIDE;

 protected:
  
  virtual WriteResult WritePacket(const char* buffer,
                                  size_t buf_len,
                                  const IPAddressNumber& self_address,
                                  const IPEndPoint& peer_address) OVERRIDE;
 private:
  UDPServerSocket* socket_;

  
  QuicBlockedWriterInterface* blocked_writer_;

  
  WriteCallback callback_;

  
  bool write_blocked_;

  base::WeakPtrFactory<QuicServerPacketWriter> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(QuicServerPacketWriter);
};

}  

#endif  
