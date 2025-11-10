#ifndef SOIL_STAGE_SCENE_COMPONENT_RENDER_RENDER_STATE_CONTAINER_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_RENDER_RENDER_STATE_CONTAINER_COMPONENT_H
#include "render_component.hpp"
#include "stage/scene/component/event/event_component.h"
#include "stage/scene/component/event/hook.h"
#include "video/render/state_container.h"
namespace soil::stage::scene::component::render {
class RenderStateContainerComponent : public RenderComponent {
 public:
  RenderStateContainerComponent();
  ~RenderStateContainerComponent() override;

  void Render(video::render::State& state) override;
  void OnEvent(const soil::stage::event::Node& event);

 private:
  void Handle(const stage::event::Component& event);

 protected:
  void OnStageChanged(Stage* stage, Stage* prevStage) override;
  void SetParent(Node* parent) override;

 private:
  video::render::StateContainer* renderStateContainer_;
  event::Hook hook_;
};
}  // namespace soil::stage::scene::component::render
#endif
