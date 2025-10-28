#ifndef SOIL_STAGE_SCENE_RENDER_PLAIN_H
#define SOIL_STAGE_SCENE_RENDER_PLAIN_H

#include "stage/scene/component/visual_component.h"
#include "stage/scene/hook/hook.h"
#include "video/render/container.h"

namespace soil::stage::scene::render {
class Plain : public hook::Hook {
 public:
  explicit Plain(video::render::Container* renderContainer);

  ~Plain() override = default;

  virtual void Perform(hook::Hook::Trigger_t trigger) override;

  void Handle(const event::Component& event) override;

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
