// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_INPUT_SYNTHETIC_GESTURE_CONTROLLER_H_
#define CONTENT_BROWSER_RENDERER_HOST_INPUT_SYNTHETIC_GESTURE_CONTROLLER_H_

#include <queue>
#include <utility>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/time/time.h"
#include "content/browser/renderer_host/input/synthetic_gesture.h"
#include "content/common/content_export.h"
#include "content/common/input/synthetic_gesture_params.h"

namespace content {

class SyntheticGestureTarget;

class CONTENT_EXPORT SyntheticGestureController {
 public:
  explicit SyntheticGestureController(
      scoped_ptr<SyntheticGestureTarget> gesture_target);
  virtual ~SyntheticGestureController();

  typedef base::Callback<void(SyntheticGesture::Result)>
      OnGestureCompleteCallback;
  void QueueSyntheticGesture(
      scoped_ptr<SyntheticGesture> synthetic_gesture,
      const OnGestureCompleteCallback& completion_callback);

  
  void Flush(base::TimeTicks timestamp);

  
  
  void OnDidFlushInput();

 private:
  void StartGesture(const SyntheticGesture& gesture);
  void StopGesture(const SyntheticGesture& gesture,
                   const OnGestureCompleteCallback& completion_callback,
                   SyntheticGesture::Result result);

  scoped_ptr<SyntheticGestureTarget> gesture_target_;
  scoped_ptr<SyntheticGesture::Result> pending_gesture_result_;

  
  
  class GestureAndCallbackQueue {
  public:
    GestureAndCallbackQueue();
    ~GestureAndCallbackQueue();
    void Push(scoped_ptr<SyntheticGesture> gesture,
        const OnGestureCompleteCallback& callback) {
      gestures_.push_back(gesture.release());
      callbacks_.push(callback);
    }
    void Pop() {
      gestures_.erase(gestures_.begin());
      callbacks_.pop();
    }
    SyntheticGesture* FrontGesture() {
      return gestures_.front();
    }
    OnGestureCompleteCallback& FrontCallback() {
      return callbacks_.front();
    }
    bool IsEmpty() {
      CHECK(gestures_.empty() == callbacks_.empty());
      return gestures_.empty();
    }
   private:
    ScopedVector<SyntheticGesture> gestures_;
    std::queue<OnGestureCompleteCallback> callbacks_;
  } pending_gesture_queue_;

  DISALLOW_COPY_AND_ASSIGN(SyntheticGestureController);
};

}  

#endif  