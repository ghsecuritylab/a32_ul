// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_ERROR_SCREEN_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_CHROMEOS_LOGIN_ERROR_SCREEN_HANDLER_H_

#include "base/cancelable_callback.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/chromeos/login/screens/error_screen_actor.h"
#include "chrome/browser/ui/webui/chromeos/login/base_screen_handler.h"
#include "chrome/browser/ui/webui/chromeos/login/network_state_informer.h"

namespace base {
class DictionaryValue;
}

namespace chromeos {

class CaptivePortalWindowProxy;
class NativeWindowDelegate;
class NetworkStateInformer;

class ErrorScreenHandler : public BaseScreenHandler,
                           public ErrorScreenActor {
 public:
  ErrorScreenHandler(
      const scoped_refptr<NetworkStateInformer>& network_state_informer);
  virtual ~ErrorScreenHandler();

  
  virtual void SetDelegate(ErrorScreenActorDelegate* delegate) OVERRIDE;
  virtual void Show(OobeDisplay::Screen parent_screen,
                    base::DictionaryValue* params) OVERRIDE;
  virtual void Show(OobeDisplay::Screen parent_screen,
                    base::DictionaryValue* params,
                    const base::Closure& on_hide) OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void FixCaptivePortal() OVERRIDE;
  virtual void ShowCaptivePortal() OVERRIDE;
  virtual void HideCaptivePortal() OVERRIDE;
  virtual void SetUIState(ErrorScreen::UIState ui_state) OVERRIDE;
  virtual void SetErrorState(ErrorScreen::ErrorState error_state,
                             const std::string& network) OVERRIDE;
  virtual void AllowGuestSignin(bool allowed) OVERRIDE;
  virtual void AllowOfflineLogin(bool allowed) OVERRIDE;
  virtual void ShowConnectingIndicator(bool show) OVERRIDE;

 private:
  
  void NetworkErrorShown();

  bool GetScreenName(OobeUI::Screen screen, std::string* name) const;

  
  void CheckAndShowScreen();

  
  void HandleShowCaptivePortal();
  void HandleHideCaptivePortal();
  void HandleLocalStateErrorPowerwashButtonClicked();
  void HandleRebootButtonClicked();
  void HandleDiagnoseButtonClicked();
  void HandleConfigureCerts();
  void HandleLaunchOobeGuestSession();

  
  virtual void RegisterMessages() OVERRIDE;

  
  virtual void DeclareLocalizedValues(LocalizedValuesBuilder* builder) OVERRIDE;
  virtual void Initialize() OVERRIDE;

  
  ErrorScreenActorDelegate* delegate_;

  
  scoped_ptr<CaptivePortalWindowProxy> captive_portal_window_proxy_;

  
  scoped_refptr<NetworkStateInformer> network_state_informer_;

  
  NativeWindowDelegate* native_window_delegate_;

  
  bool show_on_init_;

  scoped_ptr<base::Closure> on_hide_;

  base::WeakPtrFactory<ErrorScreenHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ErrorScreenHandler);
};

}  

#endif  