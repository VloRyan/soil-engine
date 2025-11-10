#ifndef SOIL_VIDEO_RENDER_RENDERABLE_GROUP_H
#define SOIL_VIDEO_RENDER_RENDERABLE_GROUP_H
#include "renderable.h"
#include "state_identifier.hpp"
#include "vector"

namespace soil::video::render {
class RenderableGroup {
 public:
  virtual ~RenderableGroup() = default;
  virtual void Insert(Renderable* renderable) = 0;
  [[nodiscard]] virtual bool Remove(Renderable* renderable) = 0;
  [[nodiscard]] virtual bool Empty() const = 0;
  [[nodiscard]] virtual const std::vector<Renderable*>& Content() const = 0;
  virtual void Render(video::render::State& state) = 0;

 protected:
  explicit RenderableGroup() = default;
};
}  // namespace soil::video::render
#endif
