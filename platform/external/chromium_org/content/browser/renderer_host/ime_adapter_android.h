// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_IME_ADAPTER_ANDROID_H_
#define CONTENT_BROWSER_RENDERER_HOST_IME_ADAPTER_ANDROID_H_

#include <jni.h>

#include "base/android/jni_weak_ref.h"

namespace content {

class RenderFrameHost;
class RenderWidgetHostImpl;
class RenderWidgetHostViewAndroid;
class WebContents;
struct NativeWebKeyboardEvent;

class ImeAdapterAndroid {
 public:
  explicit ImeAdapterAndroid(RenderWidgetHostViewAndroid* rwhva);
  ~ImeAdapterAndroid();

  
  
  
  bool SendKeyEvent(JNIEnv* env, jobject,
                    jobject original_key_event,
                    int action, int meta_state,
                    long event_time, int key_code,
                    bool is_system_key, int unicode_text);
  
  bool SendSyntheticKeyEvent(JNIEnv*,
                             jobject,
                             int event_type,
                             long timestamp_ms,
                             int native_key_code,
                             int modifiers,
                             int unicode_char);
  void SetComposingText(JNIEnv*,
                        jobject obj,
                        jobject text,
                        jstring text_str,
                        int new_cursor_pos);
  void CommitText(JNIEnv*, jobject, jstring text_str);
  void FinishComposingText(JNIEnv* env, jobject);
  void AttachImeAdapter(JNIEnv*, jobject java_object);
  void SetEditableSelectionOffsets(JNIEnv*, jobject, int start, int end);
  void SetComposingRegion(JNIEnv*, jobject, int start, int end);
  void DeleteSurroundingText(JNIEnv*, jobject, int before, int after);
  void Unselect(JNIEnv*, jobject);
  void SelectAll(JNIEnv*, jobject);
  void Cut(JNIEnv*, jobject);
  void Copy(JNIEnv*, jobject);
  void Paste(JNIEnv*, jobject);
  void ResetImeAdapter(JNIEnv*, jobject);

  
  void CancelComposition();
  void FocusedNodeChanged(bool is_editable_node);

 private:
  RenderWidgetHostImpl* GetRenderWidgetHostImpl();
  RenderFrameHost* GetFocusedFrame();
  WebContents* GetWebContents();

  RenderWidgetHostViewAndroid* rwhva_;
  JavaObjectWeakGlobalRef java_ime_adapter_;
};

bool RegisterImeAdapter(JNIEnv* env);

}  

#endif  