// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_PEPPER_GFX_CONVERSION_H_
#define CONTENT_RENDERER_PEPPER_GFX_CONVERSION_H_

#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_size.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/rect_f.h"
#include "ui/gfx/size.h"


namespace content {

inline gfx::Point PP_ToGfxPoint(const PP_Point& p) {
  return gfx::Point(p.x, p.y);
}

inline PP_Point PP_FromGfxPoint(const gfx::Point& p) {
  return PP_MakePoint(p.x(), p.y());
}

inline gfx::Rect PP_ToGfxRect(const PP_Rect& r) {
  return gfx::Rect(r.point.x, r.point.y, r.size.width, r.size.height);
}

inline gfx::RectF PP_ToGfxRectF(const PP_FloatRect& r) {
  return gfx::RectF(r.point.x, r.point.y, r.size.width, r.size.height);
}

inline PP_Rect PP_FromGfxRect(const gfx::Rect& r) {
  return PP_MakeRectFromXYWH(r.x(), r.y(), r.width(), r.height());
}

inline gfx::Size PP_ToGfxSize(const PP_Size& s) {
  return gfx::Size(s.width, s.height);
}

inline PP_Size PP_FromGfxSize(const gfx::Size& s) {
  return PP_MakeSize(s.width(), s.height());
}

}  

#endif  