#ifndef SOIL_STAGE_SCENE_RENDER_PLAIN_H
#define SOIL_STAGE_SCENE_RENDER_PLAIN_H

#include "stage/scene/component/visual_component.h"
#include "stage/scene/hook/render_hook.h"

namespace soil::stage::scene::render {
class Plain : public hook::RenderHook {
 public:
  explicit Plain(video::render::Container* renderContainer);

  ~Plain() override = default;

  void OnRender(video::render::State& state) override;

  void Handle(const event::Component& event) override;

 protected:
  void OnAdded(component::VisualComponent* component);

  void OnRemoved(component::VisualComponent* component);

  void OnStateChanged(component::VisualComponent* component);

  video::render::Container* renderContainer_;
  std::vector<component::VisualComponent*> added_;
  std::vector<component::VisualComponent*> changed_;
};
}  // namespace soil::stage::scene::render

#endif
