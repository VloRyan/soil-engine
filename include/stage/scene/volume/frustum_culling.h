#ifndef SOIL_STAGE_SCENE_VOLUME_FRUSTUM_CULLING_H
#define SOIL_STAGE_SCENE_VOLUME_FRUSTUM_CULLING_H
#include "feature.h"
#include "stage/scene/component/visual_component.h"
#include "stage/scene/viewer/node.h"
#include "world/volume/quad_tree.h"

namespace soil::stage::scene::volume {
    class FrustumCulling : public Feature {
    public:
        explicit FrustumCulling(viewer::Node* viewer, world::volume::Container* container);
        void ComponentAdded(component::Component* component) override;
        void ComponentChanged(component::Component* component) override;
        void ComponentRemoved(component::Component* component) override;
        void Update() override;
        void OnAddedToScene(Scene* scene) override;

    private:
        void updateVisibilityOnTreeNode(int index, const world::volume::Frustum* frustum);
        void setVisibility(int index, bool visible);
        world::volume::Container* container_;
        viewer::Node* viewer_;
        std::vector<bool> nodesVisibility_;
        std::vector<component::VisualComponent*> addedVisualComponents_;
    };
} // namespace soil::stage::scene::volume

#endif // SOIL_STAGE_SCENE_VOLUME_FRUSTUM_CULLING_H
