#ifndef SOIL_STAGE_SCENE_COMPONENT_RENDER_DRAWABLE_CONTAINER_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_RENDER_DRAWABLE_CONTAINER_COMPONENT_H
#include "stage/scene/component/event/event_component.h"
#include "stage/scene/component/event/hook.h"
#include "video/render/algorithm.hpp"
#include "video/render/draw/drawable_container.h"

namespace soil::stage::scene::component::render {
class DrawableContainerComponent : public Component {
 public:
  explicit DrawableContainerComponent();
  explicit DrawableContainerComponent(
      video::render::draw::DrawableContainer* container);
  ~DrawableContainerComponent() override;

  void OnEvent(const soil::stage::event::Node& event);
  video::render::draw::DrawableContainer& Container();

 private:
  void Handle(const stage::event::Component& event);

 protected:
  void OnStageChanged(Stage* stage, Stage* prevStage) override;
  void SetParent(Node* parent) override;

  void RemoveAllDependentDrawableComponents(Node* node);

 private:
  video::render::draw::DrawableContainer* container_;
  event::Hook hook_;
};
}  // namespace soil::stage::scene::component::render
#endif
