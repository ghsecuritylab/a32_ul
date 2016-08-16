// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILES_PROFILE_ANDROID_H_
#define CHROME_BROWSER_PROFILES_PROFILE_ANDROID_H_

#include <jni.h>

#include "base/android/scoped_java_ref.h"
#include "base/compiler_specific.h"
#include "base/supports_user_data.h"

class Profile;

class ProfileAndroid : public base::SupportsUserData::Data {
 public:
  static ProfileAndroid* FromProfile(Profile* profile);
  static Profile* FromProfileAndroid(jobject obj);
  static bool RegisterProfileAndroid(JNIEnv* env);

  static jobject GetLastUsedProfile(JNIEnv* env, jclass clazz);

  
  base::android::ScopedJavaLocalRef<jobject> GetOriginalProfile(
      JNIEnv* env, jobject obj);

  
  
  
  
  
  base::android::ScopedJavaLocalRef<jobject> GetOffTheRecordProfile(
      JNIEnv* env, jobject obj);

  
  jboolean HasOffTheRecordProfile(JNIEnv* env, jobject obj);

  
  jboolean IsOffTheRecord(JNIEnv* env, jobject obj);

  explicit ProfileAndroid(Profile* profile);
  virtual ~ProfileAndroid();

  base::android::ScopedJavaLocalRef<jobject> GetJavaObject();

 private:
  Profile* profile_;  
  base::android::ScopedJavaGlobalRef<jobject> obj_;
};

#endif  