// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_COMPOSITOR_IMAGE_TRANSPORT_FACTORY_H_
#define CONTENT_BROWSER_COMPOSITOR_IMAGE_TRANSPORT_FACTORY_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "cc/surfaces/surface_id_allocator.h"
#include "content/common/content_export.h"
#include "ui/gfx/native_widget_types.h"

namespace cc {
class SurfaceManager;
}

namespace gfx {
class Size;
}

namespace ui {
class ContextFactory;
class Texture;
}

namespace blink {
class WebGraphicsContext3D;
}

namespace content {
class GLHelper;

class CONTENT_EXPORT ImageTransportFactoryObserver {
 public:
  virtual ~ImageTransportFactoryObserver() {}

  
  
  
  
  
  
  virtual void OnLostResources() = 0;
};

class CONTENT_EXPORT ImageTransportFactory {
 public:
  virtual ~ImageTransportFactory() {}

  
  static void Initialize();

  
  
  static void InitializeForUnitTests(scoped_ptr<ImageTransportFactory> factory);

  
  static void Terminate();

  
  static ImageTransportFactory* GetInstance();

  
  virtual ui::ContextFactory* GetContextFactory() = 0;

  virtual gfx::GLSurfaceHandle GetSharedSurfaceHandle() = 0;
  virtual scoped_ptr<cc::SurfaceIdAllocator> CreateSurfaceIdAllocator() = 0;
  virtual cc::SurfaceManager* GetSurfaceManager() = 0;

  
  
  
  virtual GLHelper* GetGLHelper() = 0;

  virtual void AddObserver(ImageTransportFactoryObserver* observer) = 0;
  virtual void RemoveObserver(ImageTransportFactoryObserver* observer) = 0;

#if defined(OS_MACOSX)
  virtual void OnSurfaceDisplayed(int surface_id) = 0;
#endif
};

}  

#endif  