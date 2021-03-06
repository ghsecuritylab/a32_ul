// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_ANDROID_SYNCHRONOUS_COMPOSITOR_FACTORY_H_
#define CONTENT_RENDERER_ANDROID_SYNCHRONOUS_COMPOSITOR_FACTORY_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "third_party/WebKit/public/platform/WebGraphicsContext3D.h"

namespace base {
class MessageLoopProxy;
}

namespace cc {
class ContextProvider;
class OutputSurface;
}

namespace webkit {
namespace gpu {
class ContextProviderWebContext;
class WebGraphicsContext3DImpl;
}
}

namespace content {

class InputHandlerManagerClient;
class StreamTextureFactory;
class FrameSwapMessageQueue;

class SynchronousCompositorFactory {
 public:
  
  
  static void SetInstance(SynchronousCompositorFactory* instance);
  static SynchronousCompositorFactory* GetInstance();

  virtual scoped_refptr<base::MessageLoopProxy>
      GetCompositorMessageLoop() = 0;
  virtual bool RecordFullLayer() = 0;
  virtual scoped_ptr<cc::OutputSurface> CreateOutputSurface(
      int routing_id,
      scoped_refptr<FrameSwapMessageQueue> frame_swap_message_queue) = 0;

  
  virtual InputHandlerManagerClient* GetInputHandlerManagerClient() = 0;

  virtual scoped_refptr<webkit::gpu::ContextProviderWebContext>
      CreateOffscreenContextProvider(
          const blink::WebGraphicsContext3D::Attributes& attributes,
          const std::string& debug_name) = 0;
  virtual scoped_refptr<StreamTextureFactory> CreateStreamTextureFactory(
      int frame_id) = 0;
  virtual webkit::gpu::WebGraphicsContext3DImpl*
      CreateOffscreenGraphicsContext3D(
          const blink::WebGraphicsContext3D::Attributes& attributes) = 0;

 protected:
  SynchronousCompositorFactory() {}
  virtual ~SynchronousCompositorFactory() {}
};

}

#endif  
