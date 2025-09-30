#ifndef SOIL_VIDEO_RENDER_RENDERABLE_OBJECT_H
#define SOIL_VIDEO_RENDER_RENDERABLE_OBJECT_H
#include "renderable.h"

namespace soil::video::render {
class RenderableObject : public Renderable {
 public:
  RenderableObject() = default;

  ~RenderableObject() override = default;

  virtual float DistanceTo(const glm::vec3& point) = 0;

  [[nodiscard]] virtual bool IsSortable() const { return true; }
};
}  // namespace soil::video::render
#endif
