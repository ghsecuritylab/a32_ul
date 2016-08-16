// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_DOM_DISTILLER_CORE_DISTILLED_PAGE_PREFS_H_
#define COMPONENTS_DOM_DISTILLER_CORE_DISTILLED_PAGE_PREFS_H_

#include "base/macros.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"

class PrefService;

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace dom_distiller {

class DistilledPagePrefs {
 public:
  
  enum FontFamily {
#define DEFINE_FONT_FAMILY(name, value) name = value,
#include "components/dom_distiller/core/font_family_list.h"
#undef DEFINE_FONT_FAMILY
  };

  
  enum Theme {
#define DEFINE_THEME(name, value) name = value,
#include "components/dom_distiller/core/theme_list.h"
#undef DEFINE_THEME
  };

  class Observer {
   public:
    virtual void OnChangeFontFamily(FontFamily font) = 0;
    virtual void OnChangeTheme(Theme theme) = 0;
  };

  explicit DistilledPagePrefs(PrefService* pref_service);
  ~DistilledPagePrefs();

  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  void SetFontFamily(FontFamily new_font);
  
  FontFamily GetFontFamily();

  
  void SetTheme(Theme new_theme);
  
  Theme GetTheme();

  void AddObserver(Observer* obs);
  void RemoveObserver(Observer* obs);

 private:
  
  void NotifyOnChangeFontFamily(FontFamily font_family);
  
  void NotifyOnChangeTheme(Theme theme);

  PrefService* pref_service_;
  ObserverList<Observer> observers_;

  base::WeakPtrFactory<DistilledPagePrefs> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DistilledPagePrefs);
};

}  

#endif  