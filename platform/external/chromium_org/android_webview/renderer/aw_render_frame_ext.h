// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ANDROID_WEBVIEW_RENDERER_AW_RENDER_FRAME_EXT_H_
#define ANDROID_WEBVIEW_RENDERER_AW_RENDER_FRAME_EXT_H_

#include "content/public/renderer/render_frame_observer.h"

namespace android_webview {

class AwRenderFrameExt : public content::RenderFrameObserver {
 public:
  AwRenderFrameExt(content::RenderFrame* render_frame);

 private:
  virtual ~AwRenderFrameExt();

  
  virtual void DidCommitProvisionalLoad(bool is_new_navigation) OVERRIDE;
  DISALLOW_COPY_AND_ASSIGN(AwRenderFrameExt);
};

}

#endif  

