/* Copyright 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file. */

#include <sys/types.h>

#if !defined(__BIONIC__)
#include "nacl_io/kernel_intercept.h"
#include "nacl_io/kernel_wrap.h"

int cfsetispeed(struct termios *termios_p, speed_t speed) {
  termios_p->c_ispeed = speed;
  return 0;
}

#endif 
