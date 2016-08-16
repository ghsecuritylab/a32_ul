// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ANDROID_IN_PROCESS_SYNCHRONOUS_COMPOSITOR_IMPL_H_
#define CONTENT_BROWSER_ANDROID_IN_PROCESS_SYNCHRONOUS_COMPOSITOR_IMPL_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "cc/input/layer_scroll_offset_delegate.h"
#include "content/browser/android/in_process/synchronous_compositor_output_surface.h"
#include "content/common/input/input_event_ack_state.h"
#include "content/public/browser/android/synchronous_compositor.h"
#include "content/public/browser/web_contents_user_data.h"
#include "ipc/ipc_message.h"

namespace cc {
class InputHandler;
}

namespace blink {
class WebInputEvent;
}

namespace content {
class InputHandlerManager;
struct DidOverscrollParams;

class SynchronousCompositorImpl
    : public cc::LayerScrollOffsetDelegate,
      public SynchronousCompositor,
      public SynchronousCompositorOutputSurfaceDelegate,
      public WebContentsUserData<SynchronousCompositorImpl> {
 public:
  
  static SynchronousCompositorImpl* FromID(int process_id, int routing_id);
  
  
  static SynchronousCompositorImpl* FromRoutingID(int routing_id);

  InputEventAckState HandleInputEvent(const blink::WebInputEvent& input_event);

  
  virtual void SetClient(SynchronousCompositorClient* compositor_client)
      OVERRIDE;
  virtual bool InitializeHwDraw() OVERRIDE;
  virtual void ReleaseHwDraw() OVERRIDE;
  virtual scoped_ptr<cc::CompositorFrame> DemandDrawHw(
      gfx::Size surface_size,
      const gfx::Transform& transform,
      gfx::Rect viewport,
      gfx::Rect clip,
      gfx::Rect viewport_rect_for_tile_priority,
      const gfx::Transform& transform_for_tile_priority) OVERRIDE;
  virtual bool DemandDrawSw(SkCanvas* canvas) OVERRIDE;
  virtual void ReturnResources(
      const cc::CompositorFrameAck& frame_ack) OVERRIDE;
  virtual void SetMemoryPolicy(
      const SynchronousCompositorMemoryPolicy& policy) OVERRIDE;
  virtual void DidChangeRootLayerScrollOffset() OVERRIDE;

  
  virtual void DidBindOutputSurface(
      SynchronousCompositorOutputSurface* output_surface) OVERRIDE;
  virtual void DidDestroySynchronousOutputSurface(
      SynchronousCompositorOutputSurface* output_surface) OVERRIDE;
  virtual void SetContinuousInvalidate(bool enable) OVERRIDE;
  virtual void DidActivatePendingTree() OVERRIDE;

  
  virtual gfx::Vector2dF GetTotalScrollOffset() OVERRIDE;
  virtual void UpdateRootLayerState(const gfx::Vector2dF& total_scroll_offset,
                                    const gfx::Vector2dF& max_scroll_offset,
                                    const gfx::SizeF& scrollable_size,
                                    float page_scale_factor,
                                    float min_page_scale_factor,
                                    float max_page_scale_factor) OVERRIDE;
  virtual bool IsExternalFlingActive() const OVERRIDE;

  void SetInputHandler(cc::InputHandler* input_handler);
  void DidOverscroll(const DidOverscrollParams& params);
  void DidStopFlinging();

 private:
  explicit SynchronousCompositorImpl(WebContents* contents);
  virtual ~SynchronousCompositorImpl();
  friend class WebContentsUserData<SynchronousCompositorImpl>;

  void UpdateFrameMetaData(const cc::CompositorFrameMetadata& frame_info);
  void DeliverMessages();
  bool CalledOnValidThread() const;

  SynchronousCompositorClient* compositor_client_;
  SynchronousCompositorOutputSurface* output_surface_;
  WebContents* contents_;
  cc::InputHandler* input_handler_;

  base::WeakPtrFactory<SynchronousCompositorImpl> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(SynchronousCompositorImpl);
};

}  

#endif  