#ifndef SOIL_STAGE_SCENE_VOLUME_FRUSTUM_CULLING_H
#define SOIL_STAGE_SCENE_VOLUME_FRUSTUM_CULLING_H
#include "stage/scene/component/visual_component.h"
#include "stage/scene/viewer/node.h"
#include "world/volume/quad_tree.h"

namespace soil::stage::scene::volume {
class FrustumCulling : public hook::UpdateHook {
 public:
  explicit FrustumCulling(viewer::Node* viewer,
                          const world::volume::Container* container);

  void OnUpdate() override;

  void Handle(const event::Component& event) override;

 protected:
  void OnComponentAdded(component::Component* component);

 private:
  void updateVisibilityOnTreeNode(int index,
                                  const world::volume::Frustum* frustum);

  void setVisibility(int index, bool visible);

  const world::volume::Container* container_;
  viewer::Node* viewer_;
  std::vector<bool> nodesVisibility_;
  std::vector<component::VisualComponent*> addedVisualComponents_;
};
}  // namespace soil::stage::scene::volume

#endif
