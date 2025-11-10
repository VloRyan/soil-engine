#ifndef SOIL_VIDEO_RENDER_RENDERABLE_GROUP_ELEMENTS_H
#define SOIL_VIDEO_RENDER_RENDERABLE_GROUP_ELEMENTS_H
#include "renderable_group.h"
#include "vector"

namespace soil::video::render {
class RenderableGroupElements : public RenderableGroup {
 public:
  explicit RenderableGroupElements() = default;
  ~RenderableGroupElements() override = default;
  void Insert(Renderable* renderable) override;
  bool Remove(Renderable* renderable) override;
  bool Empty() const override;
  const std::vector<Renderable*>& Content() const override;
  void Render(video::render::State& state) override;

 private:
  std::vector<Renderable*> renderables_;
};
}  // namespace soil::video::render
#endif
