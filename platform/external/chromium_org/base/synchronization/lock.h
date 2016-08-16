// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SYNCHRONIZATION_LOCK_H_
#define BASE_SYNCHRONIZATION_LOCK_H_

#include "base/base_export.h"
#include "base/synchronization/lock_impl.h"
#include "base/threading/platform_thread.h"

namespace base {

class BASE_EXPORT Lock {
 public:
#if defined(NDEBUG) && !defined(DCHECK_ALWAYS_ON)
   
  Lock() : lock_() {}
  ~Lock() {}
  void Acquire() { lock_.Lock(); }
  void Release() { lock_.Unlock(); }

  
  
  
  
  bool Try() { return lock_.Try(); }

  
  void AssertAcquired() const {}
#else
  Lock();
  ~Lock();

  
  
  
  void Acquire() {
    lock_.Lock();
    CheckUnheldAndMark();
  }
  void Release() {
    CheckHeldAndUnmark();
    lock_.Unlock();
  }

  bool Try() {
    bool rv = lock_.Try();
    if (rv) {
      CheckUnheldAndMark();
    }
    return rv;
  }

  void AssertAcquired() const;
#endif  

#if defined(OS_POSIX)
  
  
  
  friend class ConditionVariable;
#elif defined(OS_WIN)
  
  
  friend class WinVistaCondVar;
#endif

 private:
#if !defined(NDEBUG) || defined(DCHECK_ALWAYS_ON)
  
  
  
  
  
  void CheckHeldAndUnmark();
  void CheckUnheldAndMark();

  
  
  base::PlatformThreadRef owning_thread_ref_;
#endif  

  
  internal::LockImpl lock_;

  DISALLOW_COPY_AND_ASSIGN(Lock);
};

class AutoLock {
 public:
  struct AlreadyAcquired {};

  explicit AutoLock(Lock& lock) : lock_(lock) {
    lock_.Acquire();
  }

  AutoLock(Lock& lock, const AlreadyAcquired&) : lock_(lock) {
    lock_.AssertAcquired();
  }

  ~AutoLock() {
    lock_.AssertAcquired();
    lock_.Release();
  }

 private:
  Lock& lock_;
  DISALLOW_COPY_AND_ASSIGN(AutoLock);
};

class AutoUnlock {
 public:
  explicit AutoUnlock(Lock& lock) : lock_(lock) {
    
    lock_.AssertAcquired();
    lock_.Release();
  }

  ~AutoUnlock() {
    lock_.Acquire();
  }

 private:
  Lock& lock_;
  DISALLOW_COPY_AND_ASSIGN(AutoUnlock);
};

}  

#endif  