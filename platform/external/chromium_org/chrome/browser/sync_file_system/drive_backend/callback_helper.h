

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_CALLBACK_HELPER_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_CALLBACK_HELPER_H_

#include "base/bind.h"
#include "base/location.h"
#include "base/logging.h"
#include "base/sequenced_task_runner.h"
#include "base/thread_task_runner_handle.h"


namespace sync_file_system {
namespace drive_backend {

namespace internal {

template <typename T>
typename base::enable_if<
    base::internal::IsMoveOnlyType<T>::value,
    base::internal::PassedWrapper<T> >::type
RebindForward(T& t) {
  return base::Passed(&t);
}

template <typename T>
typename base::enable_if<
    !base::internal::IsMoveOnlyType<T>::value,
    T&>::type
RebindForward(T& t) {
  return t;
}

template <typename T>
class CallbackHolder {
 public:
  CallbackHolder(const scoped_refptr<base::SequencedTaskRunner>& task_runner,
                 const tracked_objects::Location& from_here,
                 const base::Callback<T>& callback)
      : task_runner_(task_runner),
        from_here_(from_here),
        callback_(new base::Callback<T>(callback)) {
    DCHECK(task_runner_.get());
  }

  ~CallbackHolder() {
    base::Callback<T>* callback = callback_.release();
    if (!task_runner_->DeleteSoon(from_here_, callback))
      delete callback;
  }

  base::SequencedTaskRunner* task_runner() const { return task_runner_.get(); }
  const tracked_objects::Location& from_here() const { return from_here_; }
  const base::Callback<T>& callback() const { return *callback_; }

 private:
  scoped_refptr<base::SequencedTaskRunner> task_runner_;
  const tracked_objects::Location from_here_;
  scoped_ptr<base::Callback<T> > callback_;

  DISALLOW_COPY_AND_ASSIGN(CallbackHolder);
};

template <typename>
struct RelayToTaskRunnerHelper;

template <>
struct RelayToTaskRunnerHelper<void()> {
  static void Run(CallbackHolder<void()>* holder) {
    holder->task_runner()->PostTask(
        holder->from_here(), base::Bind(holder->callback()));
  }
};

template <typename A1>
struct RelayToTaskRunnerHelper<void(A1)> {
  static void Run(CallbackHolder<void(A1)>* holder, A1 a1) {
    holder->task_runner()->PostTask(
        holder->from_here(), base::Bind(holder->callback(), RebindForward(a1)));
  }
};

template <typename A1, typename A2>
struct RelayToTaskRunnerHelper<void(A1, A2)> {
  static void Run(CallbackHolder<void(A1, A2)>* holder, A1 a1, A2 a2) {
    holder->task_runner()->PostTask(
        holder->from_here(), base::Bind(holder->callback(), RebindForward(a1),
            RebindForward(a2)));
  }
};

template <typename A1, typename A2, typename A3>
struct RelayToTaskRunnerHelper<void(A1, A2, A3)> {
  static void Run(CallbackHolder<void(A1, A2, A3)>* holder, A1 a1, A2 a2,
      A3 a3) {
    holder->task_runner()->PostTask(
        holder->from_here(), base::Bind(holder->callback(), RebindForward(a1),
            RebindForward(a2), RebindForward(a3)));
  }
};

template <typename A1, typename A2, typename A3, typename A4>
struct RelayToTaskRunnerHelper<void(A1, A2, A3, A4)> {
  static void Run(CallbackHolder<void(A1, A2, A3, A4)>* holder, A1 a1, A2 a2,
      A3 a3, A4 a4) {
    holder->task_runner()->PostTask(
        holder->from_here(), base::Bind(holder->callback(), RebindForward(a1),
            RebindForward(a2), RebindForward(a3), RebindForward(a4)));
  }
};

template <typename A1, typename A2, typename A3, typename A4, typename A5>
struct RelayToTaskRunnerHelper<void(A1, A2, A3, A4, A5)> {
  static void Run(CallbackHolder<void(A1, A2, A3, A4, A5)>* holder, A1 a1,
      A2 a2, A3 a3, A4 a4, A5 a5) {
    holder->task_runner()->PostTask(
        holder->from_here(), base::Bind(holder->callback(), RebindForward(a1),
            RebindForward(a2), RebindForward(a3), RebindForward(a4),
            RebindForward(a5)));
  }
};

template <typename A1, typename A2, typename A3, typename A4, typename A5,
    typename A6>
struct RelayToTaskRunnerHelper<void(A1, A2, A3, A4, A5, A6)> {
  static void Run(CallbackHolder<void(A1, A2, A3, A4, A5, A6)>* holder, A1 a1,
      A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {
    holder->task_runner()->PostTask(
        holder->from_here(), base::Bind(holder->callback(), RebindForward(a1),
            RebindForward(a2), RebindForward(a3), RebindForward(a4),
            RebindForward(a5), RebindForward(a6)));
  }
};

template <typename A1, typename A2, typename A3, typename A4, typename A5,
    typename A6, typename A7>
struct RelayToTaskRunnerHelper<void(A1, A2, A3, A4, A5, A6, A7)> {
  static void Run(CallbackHolder<void(A1, A2, A3, A4, A5, A6, A7)>* holder,
      A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) {
    holder->task_runner()->PostTask(
        holder->from_here(), base::Bind(holder->callback(), RebindForward(a1),
            RebindForward(a2), RebindForward(a3), RebindForward(a4),
            RebindForward(a5), RebindForward(a6), RebindForward(a7)));
  }
};

}  

template <typename T>
base::Callback<T> RelayCallbackToTaskRunner(
    const scoped_refptr<base::SequencedTaskRunner>& task_runner,
    const tracked_objects::Location& from_here,
    const base::Callback<T>& callback) {
  DCHECK(task_runner->RunsTasksOnCurrentThread());

  if (callback.is_null())
    return base::Callback<T>();

  return base::Bind(&internal::RelayToTaskRunnerHelper<T>::Run,
                    base::Owned(new internal::CallbackHolder<T>(
                        task_runner, from_here, callback)));
}

template <typename T>
base::Callback<T> RelayCallbackToCurrentThread(
    const tracked_objects::Location& from_here,
    const base::Callback<T>& callback) {
  return RelayCallbackToTaskRunner(
      base::ThreadTaskRunnerHandle::Get(),
      from_here, callback);
}

}  
}  

#endif  