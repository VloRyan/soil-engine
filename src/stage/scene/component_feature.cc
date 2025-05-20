#include "stage/scene/component_feature.h"

namespace soil::stage::scene {
    ComponentFeature::ComponentFeature(const UpdateType type) : updateType_(type) {}

    void ComponentFeature::OnAddedToScene(Scene* scene) {}

    ComponentFeature::UpdateType ComponentFeature::GetUpdateType() const {
        return updateType_;
    }


} // namespace soil::stage::scene
