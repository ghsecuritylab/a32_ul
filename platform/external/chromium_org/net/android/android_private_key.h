// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_ANDROID_ANDROID_PRIVATE_KEY_H
#define NET_ANDROID_ANDROID_PRIVATE_KEY_H

#include <jni.h>

#include "base/android/scoped_java_ref.h"
#include "base/basictypes.h"
#include "net/base/net_export.h"


namespace net {
namespace android {

NET_EXPORT base::android::ScopedJavaLocalRef<jobject> GetKeyStore(
    jobject private_key);

NET_EXPORT bool RegisterAndroidPrivateKey(JNIEnv* env);

}  
}  

#endif  