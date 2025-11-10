#ifndef SOIL_STAGE_SCENE_COMPONENT_RENDER_RENDER_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_RENDER_RENDER_COMPONENT_H
#include "stage/scene/component/component.h"
#include "video/render/state.h"
namespace soil::stage::scene::component::render {
class RenderComponent : public Component {
 public:
  ~RenderComponent() override = default;
  virtual void Render(video::render::State& state) = 0;

 protected:
  explicit RenderComponent() : Component(Type::Render) {};
};
}  // namespace soil::stage::scene::component::render
#endif
