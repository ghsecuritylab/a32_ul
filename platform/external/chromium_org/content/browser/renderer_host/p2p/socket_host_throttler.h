// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_P2P_SOCKET_HOST_THROTTLER_H_
#define CONTENT_BROWSER_RENDERER_HOST_P2P_SOCKET_HOST_THROTTLER_H_

#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"

namespace rtc {
class RateLimiter;
class Timing;
}

namespace content {


class CONTENT_EXPORT P2PMessageThrottler {
 public:
  P2PMessageThrottler();
  virtual ~P2PMessageThrottler();

  void SetTiming(scoped_ptr<rtc::Timing> timing);
  bool DropNextPacket(size_t packet_len);
  void SetSendIceBandwidth(int bandwith_kbps);

 private:
  scoped_ptr<rtc::Timing> timing_;
  scoped_ptr<rtc::RateLimiter> rate_limiter_;

  DISALLOW_COPY_AND_ASSIGN(P2PMessageThrottler);
};

}  

#endif  