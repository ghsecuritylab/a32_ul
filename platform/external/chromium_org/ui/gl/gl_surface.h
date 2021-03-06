// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_SURFACE_H_
#define UI_GL_GL_SURFACE_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "build/build_config.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/overlay_transform.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/rect_f.h"
#include "ui/gfx/size.h"
#include "ui/gl/gl_export.h"
#include "ui/gl/gl_implementation.h"

namespace gfx {

class GLContext;
class GLImage;
class VSyncProvider;

class GL_EXPORT GLSurface : public base::RefCounted<GLSurface> {
 public:
  GLSurface();

  
  
  
  
  virtual bool Initialize();

  
  virtual void Destroy() = 0;

  virtual bool Resize(const gfx::Size& size);

  
  virtual bool Recreate();

  
  
  
  virtual bool DeferDraws();

  
  virtual bool IsOffscreen() = 0;

  
  
  virtual bool SwapBuffers() = 0;

  
  virtual gfx::Size GetSize() = 0;

  
  virtual void* GetHandle() = 0;

  
  virtual bool SupportsPostSubBuffer();

  
  
  virtual unsigned int GetBackingFrameBufferObject();

  
  virtual bool PostSubBuffer(int x, int y, int width, int height);

  
  static bool InitializeOneOff();

  
  
  static void InitializeOneOffForTests();
  static void InitializeOneOffWithMockBindingsForTests();
  static void InitializeDynamicMockBindingsForTests(GLContext* context);

  
  
  virtual bool OnMakeCurrent(GLContext* context);

  
  virtual bool SetBackbufferAllocation(bool allocated);
  virtual void SetFrontbufferAllocation(bool allocated);

  
  
  virtual void* GetShareHandle();

  
  
  virtual void* GetDisplay();

  
  virtual void* GetConfig();

  
  virtual unsigned GetFormat();

  
  
  virtual VSyncProvider* GetVSyncProvider();

  
  
  
  
  
  
  
  
  
  
  virtual bool ScheduleOverlayPlane(int z_order,
                                    OverlayTransform transform,
                                    GLImage* image,
                                    const Rect& bounds_rect,
                                    const RectF& crop_rect);

  virtual bool IsSurfaceless() const;

  
  static scoped_refptr<GLSurface> CreateViewGLSurface(
      gfx::AcceleratedWidget window);

  
  static scoped_refptr<GLSurface> CreateOffscreenGLSurface(
      const gfx::Size& size);

  static GLSurface* GetCurrent();

 protected:
  virtual ~GLSurface();
  static bool InitializeOneOffImplementation(GLImplementation impl,
                                             bool fallback_to_osmesa,
                                             bool gpu_service_logging,
                                             bool disable_gl_drawing);
  static bool InitializeOneOffInternal();
  static void SetCurrent(GLSurface* surface);

  static bool ExtensionsContain(const char* extensions, const char* name);

 private:
  friend class base::RefCounted<GLSurface>;
  friend class GLContext;

  DISALLOW_COPY_AND_ASSIGN(GLSurface);
};

class GL_EXPORT GLSurfaceAdapter : public GLSurface {
 public:
  explicit GLSurfaceAdapter(GLSurface* surface);

  virtual bool Initialize() OVERRIDE;
  virtual void Destroy() OVERRIDE;
  virtual bool Resize(const gfx::Size& size) OVERRIDE;
  virtual bool Recreate() OVERRIDE;
  virtual bool DeferDraws() OVERRIDE;
  virtual bool IsOffscreen() OVERRIDE;
  virtual bool SwapBuffers() OVERRIDE;
  virtual bool PostSubBuffer(int x, int y, int width, int height) OVERRIDE;
  virtual bool SupportsPostSubBuffer() OVERRIDE;
  virtual gfx::Size GetSize() OVERRIDE;
  virtual void* GetHandle() OVERRIDE;
  virtual unsigned int GetBackingFrameBufferObject() OVERRIDE;
  virtual bool OnMakeCurrent(GLContext* context) OVERRIDE;
  virtual bool SetBackbufferAllocation(bool allocated) OVERRIDE;
  virtual void SetFrontbufferAllocation(bool allocated) OVERRIDE;
  virtual void* GetShareHandle() OVERRIDE;
  virtual void* GetDisplay() OVERRIDE;
  virtual void* GetConfig() OVERRIDE;
  virtual unsigned GetFormat() OVERRIDE;
  virtual VSyncProvider* GetVSyncProvider() OVERRIDE;
  virtual bool ScheduleOverlayPlane(int z_order,
                                    OverlayTransform transform,
                                    GLImage* image,
                                    const Rect& bounds_rect,
                                    const RectF& crop_rect) OVERRIDE;
  virtual bool IsSurfaceless() const OVERRIDE;

  GLSurface* surface() const { return surface_.get(); }

 protected:
  virtual ~GLSurfaceAdapter();

 private:
  scoped_refptr<GLSurface> surface_;

  DISALLOW_COPY_AND_ASSIGN(GLSurfaceAdapter);
};

}  

#endif  
