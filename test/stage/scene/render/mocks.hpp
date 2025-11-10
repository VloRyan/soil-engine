#ifndef TEST_STAGE_SCENE_RENDER_MOCKS
#define TEST_STAGE_SCENE_RENDER_MOCKS
#include "video/render/container.h"
#include "video/render/renderable_group.h"
#include "video/render/renderable_group_elements.h"

namespace soil::stage::scene::render {
class RenderContainerMock : public video::render::Container {
 public:
  RenderContainerMock() : Container() {}
  void Add(video::render::RenderableObject* renderable,
           const video::render::RenderDef& state) override {
    Calls.Add++;
    video::render::Container::Add(renderable, state);
  }

  bool Remove(video::render::RenderableObject* renderable,
              const video::render::RenderDef& state) override {
    Calls.Remove++;
    return video::render::Container::Remove(renderable, state);
  }
  struct Calls_t {
    int Add{0};
    int Remove{0};
  } Calls;
};

class VisualComponentMock : public component::VisualComponent,
                            public video::render::RenderableObject {
 public:
  explicit VisualComponentMock(render::Type renderType, bool opaque = true,
                               bool visible = true)
      : VisualComponent(renderType, opaque, visible) {}
  video::render::RenderableObject* GetRenderable() override { return this; }
  // void Render(video::render::State& state) override {}
  float DistanceTo(const glm::vec3& point) override { return 0.F; }

  video::render::RenderableGroup* NewGroup() const override {
    return new video::render::RenderableGroupElements();
  }
  const video::vertex::Vao* Vao() const override { return nullptr; }

  video::render::DrawMode DrawMode() const override {
    return video::render::DrawMode::Quads;
  }
  void ApplyData(const video::render::data::IWriter& writer,
                 soil::video::render::State& state) const override {}
  void Render(video::render::State& state, int count) override {}
};

}  // namespace soil::stage::scene::render

#endif
