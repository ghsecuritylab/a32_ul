// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOM_DISTILLER_TAB_UTILS_H_
#define CHROME_BROWSER_DOM_DISTILLER_TAB_UTILS_H_

namespace content {
class WebContents;
}  

void DistillCurrentPageAndView(content::WebContents* old_web_contents);

#endif  
