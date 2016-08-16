// Copyright 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_RESOURCES_LAYER_UPDATER_H_
#define CC_RESOURCES_LAYER_UPDATER_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "cc/base/cc_export.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/vector2d.h"

namespace cc {

class PrioritizedResource;
class PrioritizedResourceManager;
class ResourceUpdateQueue;
class TextureManager;

class CC_EXPORT LayerUpdater : public base::RefCounted<LayerUpdater> {
 public:
  
  class CC_EXPORT Resource {
   public:
    virtual ~Resource();

    PrioritizedResource* texture() { return texture_.get(); }
    
    
    virtual void Update(ResourceUpdateQueue* queue,
                        const gfx::Rect& source_rect,
                        const gfx::Vector2d& dest_offset,
                        bool partial_update) = 0;

   protected:
    explicit Resource(scoped_ptr<PrioritizedResource> texture);

   private:
    scoped_ptr<PrioritizedResource> texture_;

    DISALLOW_COPY_AND_ASSIGN(Resource);
  };

  LayerUpdater() {}

  virtual scoped_ptr<Resource> CreateResource(
      PrioritizedResourceManager* manager) = 0;
  virtual void PrepareToUpdate(const gfx::Size& content_size,
                               const gfx::Rect& paint_rect,
                               const gfx::Size& tile_size,
                               float contents_width_scale,
                               float contents_height_scale) {}
  virtual void ReduceMemoryUsage() {}

  
  
  virtual void SetOpaque(bool opaque) {}
  
  
  virtual void SetFillsBoundsCompletely(bool fills_bounds) {}
  virtual void SetBackgroundColor(SkColor background_color) {}

 protected:
  virtual ~LayerUpdater() {}

 private:
  friend class base::RefCounted<LayerUpdater>;

  DISALLOW_COPY_AND_ASSIGN(LayerUpdater);
};

}  

#endif  