#ifndef SOIL_STAGE_SCENE_VOLUME_FRUSTUM_CULLING_H
#define SOIL_STAGE_SCENE_VOLUME_FRUSTUM_CULLING_H
#include "stage/event/node.h"
#include "stage/hook/event_hook.hpp"
#include "stage/hook/trigger_hook.h"
#include "stage/scene/component/render/drawable_component.h"
#include "stage/scene/component/world/world_component.h"
#include "stage/scene/viewer/node.h"
#include "world/volume/container.h"

namespace soil::stage::scene::volume {
class FrustumCulling : public hook::EventHook<event::Node>,
                       public hook::TriggerHook {
 public:
  explicit FrustumCulling(viewer::Node* viewer,
                          const component::world::WorldComponent* world);

  void OnEvent(const event::Node& event) override;
  void OnTrigger(const TriggerPoint& point) override;

 protected:
  void OnComponentAdded(component::Component* component);

 private:
  void updateVisibilityOnTreeNode(int index,
                                  const soil::world::volume::Frustum* frustum);

  void setVisibility(int index, bool visible);

  const component::world::WorldComponent* world_;
  viewer::Node* viewer_;
  std::vector<bool> nodesVisibility_;
  std::vector<component::DrawableComponent*> addedRenderables_;
};
}  // namespace soil::stage::scene::volume

#endif
