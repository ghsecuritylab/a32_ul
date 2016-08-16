// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_WIN_H_
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_WIN_H_

#include <windows.h>

#include <list>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/message_loop/message_pump.h"
#include "base/message_loop/message_pump_dispatcher.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "base/win/scoped_handle.h"

namespace base {

class BASE_EXPORT MessagePumpWin : public MessagePump {
 public:
  MessagePumpWin() : have_work_(0), state_(NULL) {}
  virtual ~MessagePumpWin() {}

  
  void RunWithDispatcher(Delegate* delegate, MessagePumpDispatcher* dispatcher);

  
  virtual void Run(Delegate* delegate) { RunWithDispatcher(delegate, NULL); }
  virtual void Quit();

 protected:
  struct RunState {
    Delegate* delegate;
    MessagePumpDispatcher* dispatcher;

    
    bool should_quit;

    
    int run_depth;
  };

  virtual void DoRunLoop() = 0;
  int GetCurrentDelay() const;

  
  TimeTicks delayed_work_time_;

  
  
  
  LONG have_work_;

  
  RunState* state_;
};

class BASE_EXPORT MessagePumpForUI : public MessagePumpWin {
 public:
  
  static const int kMessageFilterCode = 0x5001;

  MessagePumpForUI();
  virtual ~MessagePumpForUI();

  
  virtual void ScheduleWork();
  virtual void ScheduleDelayedWork(const TimeTicks& delayed_work_time);

 private:
  static LRESULT CALLBACK WndProcThunk(HWND window_handle,
                                       UINT message,
                                       WPARAM wparam,
                                       LPARAM lparam);
  virtual void DoRunLoop();
  void InitMessageWnd();
  void WaitForWork();
  void HandleWorkMessage();
  void HandleTimerMessage();
  bool ProcessNextWindowsMessage();
  bool ProcessMessageHelper(const MSG& msg);
  bool ProcessPumpReplacementMessage();

  
  ATOM atom_;

  
  HWND message_hwnd_;
};

class BASE_EXPORT MessagePumpForIO : public MessagePumpWin {
 public:
  struct IOContext;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  class IOHandler {
   public:
    virtual ~IOHandler() {}
    
    
    
    
    virtual void OnIOCompleted(IOContext* context, DWORD bytes_transfered,
                               DWORD error) = 0;
  };

  
  
  
  
  class IOObserver {
   public:
    IOObserver() {}

    virtual void WillProcessIOEvent() = 0;
    virtual void DidProcessIOEvent() = 0;

   protected:
    virtual ~IOObserver() {}
  };

  
  
  
  
  
  
  
  
  
  struct IOContext {
    OVERLAPPED overlapped;
    IOHandler* handler;
  };

  MessagePumpForIO();
  virtual ~MessagePumpForIO() {}

  
  virtual void ScheduleWork();
  virtual void ScheduleDelayedWork(const TimeTicks& delayed_work_time);

  
  
  
  void RegisterIOHandler(HANDLE file_handle, IOHandler* handler);

  
  
  
  
  bool RegisterJobObject(HANDLE job_handle, IOHandler* handler);

  
  
  
  
  
  
  
  
  
  bool WaitForIOCompletion(DWORD timeout, IOHandler* filter);

  void AddIOObserver(IOObserver* obs);
  void RemoveIOObserver(IOObserver* obs);

 private:
  struct IOItem {
    IOHandler* handler;
    IOContext* context;
    DWORD bytes_transfered;
    DWORD error;

    
    
    
    bool has_valid_io_context;
  };

  virtual void DoRunLoop();
  void WaitForWork();
  bool MatchCompletedIOItem(IOHandler* filter, IOItem* item);
  bool GetIOItem(DWORD timeout, IOItem* item);
  bool ProcessInternalIOItem(const IOItem& item);
  void WillProcessIOEvent();
  void DidProcessIOEvent();

  
  
  
  static ULONG_PTR HandlerToKey(IOHandler* handler, bool has_valid_io_context);

  
  static IOHandler* KeyToHandler(ULONG_PTR key, bool* has_valid_io_context);

  
  win::ScopedHandle port_;
  
  
  std::list<IOItem> completed_io_;

  ObserverList<IOObserver> io_observers_;
};

}  

#endif  