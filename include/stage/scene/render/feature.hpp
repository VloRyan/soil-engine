#ifndef SOIL_STAGE_SCENE_RENDER_FEATURE_HPP
#define SOIL_STAGE_SCENE_RENDER_FEATURE_HPP
#include "stage/scene/component/visual_component.h"
#include "video/render/container.h"

namespace soil::stage::scene::render {
    class Feature {
    public:
        virtual ~Feature() = default;
        virtual void ComponentAdded(component::VisualComponent* component) = 0;
        virtual void ComponentChanged(component::VisualComponent* component) = 0;
        virtual void ComponentRemoved(component::VisualComponent* component) = 0;
        virtual void Update(video::render::Container* container) = 0;

    protected:
        Feature() = default;
    };
} // namespace soil::stage::scene::render
#endif // FEATURE_HPP
