#ifndef TEST_STAGE_SCENE_RENDER_MOCKS
#define TEST_STAGE_SCENE_RENDER_MOCKS
#include "video/render/container.h"
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
}  // namespace soil::stage::scene::render

#endif
