#ifndef SOIL_STAGE_SCENE_RENDER_FEATURE_HPP
#define SOIL_STAGE_SCENE_RENDER_FEATURE_HPP
#include "stage/scene/component/visual_component.h"
#include "stage/scene/component_feature.h"
#include "video/render/container.h"

namespace soil::stage::scene::render {
    class Feature : public ComponentFeature {
    public:
        ~Feature() override = default;

        void OnAddedToScene(Scene* scene) override;

    protected:
        explicit Feature() : ComponentFeature(UpdateType::Render), container_(nullptr) {}

        virtual component::VisualComponent* CastVisualComponent(component::Component* component);
        virtual bool IsVisualComponentOf(component::Component* component, Type renderType);

        video::render::Container* container_;
    };
} // namespace soil::stage::scene::render
#endif // SOIL_STAGE_SCENE_RENDER_FEATURE_HPP
