// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_POSIX_EINTR_WRAPPER_H_
#define BASE_POSIX_EINTR_WRAPPER_H_

#include "build/build_config.h"

#if defined(OS_POSIX)

#include <errno.h>

#if defined(NDEBUG)

#define HANDLE_EINTR(x) ({ \
  typeof(x) eintr_wrapper_result; \
  do { \
    eintr_wrapper_result = (x); \
  } while (eintr_wrapper_result == -1 && errno == EINTR); \
  eintr_wrapper_result; \
})

#else

#define HANDLE_EINTR(x) ({ \
  int eintr_wrapper_counter = 0; \
  typeof(x) eintr_wrapper_result; \
  do { \
    eintr_wrapper_result = (x); \
  } while (eintr_wrapper_result == -1 && errno == EINTR && \
           eintr_wrapper_counter++ < 100); \
  eintr_wrapper_result; \
})

#endif  

#define IGNORE_EINTR(x) ({ \
  typeof(x) eintr_wrapper_result; \
  do { \
    eintr_wrapper_result = (x); \
    if (eintr_wrapper_result == -1 && errno == EINTR) { \
      eintr_wrapper_result = 0; \
    } \
  } while (0); \
  eintr_wrapper_result; \
})

#else

#define HANDLE_EINTR(x) (x)
#define IGNORE_EINTR(x) (x)

#endif  

#endif  
