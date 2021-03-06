// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_MOUSE_CLAMPING_FILTER_H_
#define REMOTING_HOST_MOUSE_CLAMPING_FILTER_H_

#include "base/compiler_specific.h"
#include "remoting/protocol/mouse_input_filter.h"
#include "remoting/protocol/video_stub.h"

namespace remoting {

class MouseClampingFilter : public protocol::VideoStub {
 public:
  explicit MouseClampingFilter(protocol::InputStub* input_stub);
  virtual ~MouseClampingFilter();

  protocol::InputStub* input_filter() { return &input_filter_; }

  void set_video_stub(protocol::VideoStub* video_stub) {
    video_stub_ = video_stub;
  }

  
  virtual void ProcessVideoPacket(scoped_ptr<VideoPacket> video_packet,
                                  const base::Closure& done) OVERRIDE;

 private:
  
  protocol::MouseInputFilter input_filter_;

  protocol::VideoStub* video_stub_;

  DISALLOW_COPY_AND_ASSIGN(MouseClampingFilter);
};

}  

#endif  
