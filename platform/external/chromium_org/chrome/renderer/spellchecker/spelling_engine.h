// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_SPELLCHECKER_SPELLING_ENGINE_H_
#define CHROME_RENDERER_SPELLCHECKER_SPELLING_ENGINE_H_

#include <string>
#include <vector>

#include "base/files/file.h"
#include "base/strings/string16.h"

class SpellingEngine* CreateNativeSpellingEngine();

class SpellingEngine {
 public:
  virtual ~SpellingEngine() {}

  
  
  virtual void Init(base::File bdict_file) = 0;
  virtual bool InitializeIfNeeded() = 0;
  virtual bool IsEnabled() = 0;
  virtual bool CheckSpelling(const base::string16& word_to_check, int tag) = 0;
  virtual void FillSuggestionList(
      const base::string16& wrong_word,
      std::vector<base::string16>* optional_suggestions) = 0;
};

#endif  
