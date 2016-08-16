// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GAMEPAD_GAMEPAD_PROVIDER_H_
#define CONTENT_BROWSER_GAMEPAD_GAMEPAD_PROVIDER_H_

#include <utility>
#include <vector>

#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/shared_memory.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/synchronization/lock.h"
#include "base/system_monitor/system_monitor.h"
#include "content/common/content_export.h"
#include "third_party/WebKit/public/platform/WebGamepads.h"

namespace base {
class MessageLoopProxy;
class Thread;
}

namespace content {

class GamepadDataFetcher;
struct GamepadHardwareBuffer;

class CONTENT_EXPORT GamepadProvider :
  public base::SystemMonitor::DevicesChangedObserver {
 public:
  GamepadProvider();

  
  explicit GamepadProvider(scoped_ptr<GamepadDataFetcher> fetcher);

  virtual ~GamepadProvider();

  
  
  base::SharedMemoryHandle GetSharedMemoryHandleForProcess(
      base::ProcessHandle renderer_process);

  void GetCurrentGamepadData(blink::WebGamepads* data);

  
  
  void Pause();
  void Resume();

  
  
  void RegisterForUserGesture(const base::Closure& closure);

  
  virtual void OnDevicesChanged(base::SystemMonitor::DeviceType type) OVERRIDE;

 private:
  void Initialize(scoped_ptr<GamepadDataFetcher> fetcher);

  
  
  void DoInitializePollingThread(scoped_ptr<GamepadDataFetcher> fetcher);

  
  
  void SendPauseHint(bool paused);

  
  void DoPoll();
  void ScheduleDoPoll();

  void OnGamepadConnectionChange(bool connected,
                                 int index,
                                 const blink::WebGamepad& pad);
  void DispatchGamepadConnectionChange(bool connected,
                                       int index,
                                       const blink::WebGamepad& pad);

  GamepadHardwareBuffer* SharedMemoryAsHardwareBuffer();

  
  void CheckForUserGesture();

  enum { kDesiredSamplingIntervalMs = 16 };

  
  
  base::Lock is_paused_lock_;
  bool is_paused_;

  
  
  
  bool have_scheduled_do_poll_;

  
  
  
  
  
  base::Lock user_gesture_lock_;
  struct ClosureAndThread {
    ClosureAndThread(const base::Closure& c,
                     const scoped_refptr<base::MessageLoopProxy>& m);
    ~ClosureAndThread();

    base::Closure closure;
    scoped_refptr<base::MessageLoopProxy> message_loop;
  };
  typedef std::vector<ClosureAndThread> UserGestureObserverVector;
  UserGestureObserverVector user_gesture_observers_;

  
  
  
  
  
  base::Lock devices_changed_lock_;
  bool devices_changed_;

  bool ever_had_user_gesture_;

  class PadState {
   public:
    PadState() {
      SetDisconnected();
    }

    bool Match(const blink::WebGamepad& pad) const;
    void SetPad(const blink::WebGamepad& pad);
    void SetDisconnected();
    void AsWebGamepad(blink::WebGamepad* pad);

    bool connected() const { return connected_; }

   private:
    bool connected_;
    unsigned axes_length_;
    unsigned buttons_length_;
    blink::WebUChar id_[blink::WebGamepad::idLengthCap];
    blink::WebUChar mapping_[blink::WebGamepad::mappingLengthCap];
  };

  
  scoped_ptr<PadState[]> pad_states_;

  
  scoped_ptr<GamepadDataFetcher> data_fetcher_;

  base::Lock shared_memory_lock_;
  base::SharedMemory gamepad_shared_memory_;

  
  scoped_ptr<base::Thread> polling_thread_;

  static GamepadProvider* instance_;

  DISALLOW_COPY_AND_ASSIGN(GamepadProvider);
};

}  

#endif  