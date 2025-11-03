#ifndef SOIL_STAGE_SCENE_RENDER_PLAIN_H
#define SOIL_STAGE_SCENE_RENDER_PLAIN_H

#include "stage/event/node.h"
#include "stage/hook/event_hook.hpp"
#include "stage/hook/trigger_hook.h"
#include "stage/scene/component/visual_component.h"
#include "video/render/container.h"

namespace soil::stage::scene::render {
class Plain : public hook::EventHook<event::Node>, public hook::TriggerHook {
 public:
  explicit Plain(video::render::Container* renderContainer);

  ~Plain() override = default;

  void OnTrigger(hook::TriggerHook::TriggerType trigger) override;
  void OnEvent(const soil::stage::event::Node& event) override;
  void Handle(const event::Component& event);

 protected:
  void OnAdded(component::VisualComponent* component);

  void OnRemoved(component::VisualComponent* component);

  void OnChanged(component::VisualComponent* component);

  void UpdateOpacity(bool isOpaque, int index,
                     video::render::RenderableObject* object);

  video::render::Container* renderContainer_;
  std::vector<component::VisualComponent*> added_;
  std::vector<component::VisualComponent*> changed_;
};
}  // namespace soil::stage::scene::render

#endif
