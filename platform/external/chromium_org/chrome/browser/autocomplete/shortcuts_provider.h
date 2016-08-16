// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_SHORTCUTS_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_SHORTCUTS_PROVIDER_H_

#include <map>
#include <set>
#include <string>

#include "base/gtest_prod_util.h"
#include "chrome/browser/autocomplete/shortcuts_backend.h"
#include "components/omnibox/autocomplete_provider.h"

class Profile;
class ShortcutsProviderTest;

class ShortcutsProvider
    : public AutocompleteProvider,
      public ShortcutsBackend::ShortcutsBackendObserver {
 public:
  explicit ShortcutsProvider(Profile* profile);

  
  
  virtual void Start(const AutocompleteInput& input,
                     bool minimal_changes) OVERRIDE;

  virtual void DeleteMatch(const AutocompleteMatch& match) OVERRIDE;

 private:
  friend class ClassifyTest;
  friend class ShortcutsProviderTest;
  FRIEND_TEST_ALL_PREFIXES(ShortcutsProviderTest, CalculateScore);

  typedef std::multimap<base::char16, base::string16> WordMap;

  virtual ~ShortcutsProvider();

  
  virtual void OnShortcutsLoaded() OVERRIDE;

  
  void GetMatches(const AutocompleteInput& input);

  
  
  
  
  
  AutocompleteMatch ShortcutToACMatch(
      const history::ShortcutsDatabase::Shortcut& shortcut,
      int relevance,
      const AutocompleteInput& input,
      const base::string16& fixed_up_input_text);

  
  
  
  
  static WordMap CreateWordMapForString(const base::string16& text);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static ACMatchClassifications ClassifyAllMatchesInString(
      const base::string16& find_text,
      const WordMap& find_words,
      const base::string16& text,
      const ACMatchClassifications& original_class);

  
  
  ShortcutsBackend::ShortcutMap::const_iterator FindFirstMatch(
      const base::string16& keyword,
      ShortcutsBackend* backend);

  int CalculateScore(const base::string16& terms,
                     const history::ShortcutsDatabase::Shortcut& shortcut,
                     int max_relevance);

  
  static const int kShortcutsProviderDefaultMaxRelevance;

  Profile* profile_;
  std::string languages_;
  bool initialized_;
};

#endif  