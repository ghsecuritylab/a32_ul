// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_WEBMESSAGEPORTCHANNEL_IMPL_H_
#define CONTENT_CHILD_WEBMESSAGEPORTCHANNEL_IMPL_H_

#include <queue>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "base/synchronization/lock.h"
#include "ipc/ipc_listener.h"
#include "third_party/WebKit/public/platform/WebMessagePortChannel.h"

namespace base {
class MessageLoopProxy;
}

namespace content {
class ChildThread;

class WebMessagePortChannelImpl
    : public blink::WebMessagePortChannel,
      public IPC::Listener,
      public base::RefCountedThreadSafe<WebMessagePortChannelImpl> {
 public:
  explicit WebMessagePortChannelImpl(
      const scoped_refptr<base::MessageLoopProxy>& child_thread_loop);
  WebMessagePortChannelImpl(
      int route_id,
      int message_port_id,
      const scoped_refptr<base::MessageLoopProxy>& child_thread_loop);

  static void CreatePair(
      const scoped_refptr<base::MessageLoopProxy>& child_thread_loop,
      blink::WebMessagePortChannel** channel1,
      blink::WebMessagePortChannel** channel2);

  
  
  static std::vector<int> ExtractMessagePortIDs(
      blink::WebMessagePortChannelArray* channels);

  
  
  void QueueMessages();
  int message_port_id() const { return message_port_id_; }

 private:
  friend class base::RefCountedThreadSafe<WebMessagePortChannelImpl>;
  virtual ~WebMessagePortChannelImpl();

  
  virtual void setClient(blink::WebMessagePortChannelClient* client);
  virtual void destroy();
  virtual void postMessage(const blink::WebString& message,
                           blink::WebMessagePortChannelArray* channels);
  virtual bool tryGetMessage(blink::WebString* message,
                             blink::WebMessagePortChannelArray& channels);

  void Init();
  void Entangle(scoped_refptr<WebMessagePortChannelImpl> channel);
  void Send(IPC::Message* message);
  void PostMessage(const base::string16& message,
                   blink::WebMessagePortChannelArray* channels);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  void OnMessage(const base::string16& message,
                 const std::vector<int>& sent_message_port_ids,
                 const std::vector<int>& new_routing_ids);
  void OnMessagesQueued();

  struct Message {
    Message();
    ~Message();

    base::string16 message;
    std::vector<WebMessagePortChannelImpl*> ports;
  };

  typedef std::queue<Message> MessageQueue;
  MessageQueue message_queue_;

  blink::WebMessagePortChannelClient* client_;
  base::Lock lock_;  

  int route_id_;  
  int message_port_id_;  
  scoped_refptr<base::MessageLoopProxy> child_thread_loop_;

  DISALLOW_COPY_AND_ASSIGN(WebMessagePortChannelImpl);
};

}  

#endif  