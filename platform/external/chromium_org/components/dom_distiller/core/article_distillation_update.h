// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_DOM_DISTILLER_CORE_ARTICLE_DISTILLATION_UPDATE_H_
#define COMPONENTS_DOM_DISTILLER_CORE_ARTICLE_DISTILLATION_UPDATE_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "components/dom_distiller/core/proto/distilled_page.pb.h"

namespace dom_distiller {

class ArticleDistillationUpdate {
 public:
  typedef base::RefCountedData<DistilledPageProto> RefCountedPageProto;

  ArticleDistillationUpdate(
      const std::vector<scoped_refptr<RefCountedPageProto> >& pages,
      bool has_next_page,
      bool has_prev_page);
  ~ArticleDistillationUpdate();

  
  const DistilledPageProto& GetDistilledPage(size_t index) const;

  
  size_t GetPagesSize() const { return pages_.size(); }

  
  
  
  bool HasNextPage() const { return has_next_page_; }

  
  
  
  bool HasPrevPage() const { return has_prev_page_; }

 private:
  bool has_next_page_;
  bool has_prev_page_;
  
  std::vector<scoped_refptr<RefCountedPageProto> > pages_;
};

}  

#endif  