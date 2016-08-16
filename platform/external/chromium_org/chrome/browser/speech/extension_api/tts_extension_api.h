// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPEECH_EXTENSION_API_TTS_EXTENSION_API_H_
#define CHROME_BROWSER_SPEECH_EXTENSION_API_TTS_EXTENSION_API_H_

#include <string>

#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/speech/tts_controller.h"
#include "extensions/browser/browser_context_keyed_api_factory.h"

namespace content {
class BrowserContext;
}

const char *TtsEventTypeToString(TtsEventType event_type);
TtsEventType TtsEventTypeFromString(const std::string& str);

namespace extensions {

class TtsSpeakFunction : public ChromeAsyncExtensionFunction {
 private:
  virtual ~TtsSpeakFunction() {}
  virtual bool RunAsync() OVERRIDE;
  DECLARE_EXTENSION_FUNCTION("tts.speak", TTS_SPEAK)
};

class TtsStopSpeakingFunction : public ChromeSyncExtensionFunction {
 private:
  virtual ~TtsStopSpeakingFunction() {}
  virtual bool RunSync() OVERRIDE;
  DECLARE_EXTENSION_FUNCTION("tts.stop", TTS_STOP)
};

class TtsPauseFunction : public ChromeSyncExtensionFunction {
 private:
  virtual ~TtsPauseFunction() {}
  virtual bool RunSync() OVERRIDE;
  DECLARE_EXTENSION_FUNCTION("tts.pause", TTS_PAUSE)
};

class TtsResumeFunction : public ChromeSyncExtensionFunction {
 private:
  virtual ~TtsResumeFunction() {}
  virtual bool RunSync() OVERRIDE;
  DECLARE_EXTENSION_FUNCTION("tts.resume", TTS_RESUME)
};

class TtsIsSpeakingFunction : public ChromeSyncExtensionFunction {
 private:
  virtual ~TtsIsSpeakingFunction() {}
  virtual bool RunSync() OVERRIDE;
  DECLARE_EXTENSION_FUNCTION("tts.isSpeaking", TTS_ISSPEAKING)
};

class TtsGetVoicesFunction : public ChromeSyncExtensionFunction {
 private:
  virtual ~TtsGetVoicesFunction() {}
  virtual bool RunSync() OVERRIDE;
  DECLARE_EXTENSION_FUNCTION("tts.getVoices", TTS_GETVOICES)
};

class TtsAPI : public BrowserContextKeyedAPI {
 public:
  explicit TtsAPI(content::BrowserContext* context);
  virtual ~TtsAPI();

  
  static BrowserContextKeyedAPIFactory<TtsAPI>* GetFactoryInstance();

 private:
  friend class BrowserContextKeyedAPIFactory<TtsAPI>;

  
  static const char* service_name() {
    return "TtsAPI";
  }
  static const bool kServiceIsNULLWhileTesting = true;
};

}  

#endif  