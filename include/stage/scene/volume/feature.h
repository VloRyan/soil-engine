#ifndef SOIL_STAGE_SCENE_VOLUME_FEATURE_H
#define SOIL_STAGE_SCENE_VOLUME_FEATURE_H
#include "stage/scene/component_feature.h"

namespace soil::stage::scene::volume {
    class Feature : public ComponentFeature {
    public:
        ~Feature() override = default;

    protected:
        explicit Feature();
    };
} // namespace soil::stage::scene::volume

#endif // FEATURE_H
