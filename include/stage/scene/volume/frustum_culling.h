#ifndef SOIL_STAGE_SCENE_VOLUME_FRUSTUM_CULLING_H
#define SOIL_STAGE_SCENE_VOLUME_FRUSTUM_CULLING_H
#include "stage/scene/component/visual_component.h"
#include "stage/scene/hook/hook.h"
#include "stage/scene/viewer/node.h"
#include "stage/scene/world/world_node.h"
#include "world/volume/container.h"

namespace soil::stage::scene::volume {
class FrustumCulling : public hook::Hook {
 public:
  explicit FrustumCulling(viewer::Node* viewer, const world::WorldNode* world);

  void Perform(hook::Hook::Trigger_t trigger) override;

  void Handle(const event::Component& event) override;

 protected:
  void OnComponentAdded(component::Component* component);

 private:
  void updateVisibilityOnTreeNode(int index,
                                  const soil::world::volume::Frustum* frustum);

  void setVisibility(int index, bool visible);

  const world::WorldNode* world_;
  viewer::Node* viewer_;
  std::vector<bool> nodesVisibility_;
  std::vector<component::VisualComponent*> addedVisualComponents_;
};
}  // namespace soil::stage::scene::volume

#endif
