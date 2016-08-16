// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_THUMBNAILS_THUMBNAIL_LIST_SOURCE_H_
#define CHROME_BROWSER_THUMBNAILS_THUMBNAIL_LIST_SOURCE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "components/history/core/browser/history_types.h"
#include "content/public/browser/url_data_source.h"

class Profile;

namespace base {
class RefCountedMemory;
}

namespace thumbnails {
class ThumbnailService;
}

class ThumbnailListSource : public content::URLDataSource {
 public:
  explicit ThumbnailListSource(Profile* profile);

  
  virtual std::string GetSource() const OVERRIDE;
  virtual void StartDataRequest(
      const std::string& path,
      int render_process_id,
      int render_frame_id,
      const content::URLDataSource::GotDataCallback& callback) OVERRIDE;
  virtual std::string GetMimeType(const std::string& path) const OVERRIDE;
  virtual base::MessageLoop* MessageLoopForRequestPath(
      const std::string& path) const OVERRIDE;
  virtual bool ShouldServiceRequest(
      const net::URLRequest* request) const OVERRIDE;
  virtual bool ShouldReplaceExistingSource() const OVERRIDE;

 private:
  virtual ~ThumbnailListSource();

  void OnMostVisitedURLsAvailable(
    const content::URLDataSource::GotDataCallback& callback,
    const history::MostVisitedURLList& mvurl_list);

  
  scoped_refptr<thumbnails::ThumbnailService> thumbnail_service_;

  
  Profile* const profile_;

  
  base::WeakPtrFactory<ThumbnailListSource> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ThumbnailListSource);
};


#endif  