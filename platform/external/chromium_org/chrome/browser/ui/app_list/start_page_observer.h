// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_START_PAGE_OBSERVER_H_
#define CHROME_BROWSER_UI_APP_LIST_START_PAGE_OBSERVER_H_

#include "base/strings/string16.h"
#include "ui/app_list/speech_ui_model_observer.h"

namespace app_list {

class StartPageObserver {
 public:
  
  virtual void OnSpeechResult(const base::string16& query, bool is_final) = 0;

  
  virtual void OnSpeechSoundLevelChanged(int16 level) = 0;

  
  virtual void OnSpeechRecognitionStateChanged(
      SpeechRecognitionState new_state) = 0;

 protected:
  virtual ~StartPageObserver() {}
};

}  

#endif  