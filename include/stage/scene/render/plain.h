#ifndef SOIL_STAGE_SCENE_RENDER_DEFAULT_CONTAINER_H
#define SOIL_STAGE_SCENE_RENDER_DEFAULT_CONTAINER_H
#include "feature.h"

namespace soil::stage::scene::render {
    class Plain : public Feature {
    public:
        Plain() = default;
        ~Plain() override = default;
        void Update() override;
        void ComponentAdded(component::Component* component) override;
        void ComponentRemoved(component::Component* component) override;
        void ComponentChanged(component::Component* component) override;

    protected:
        std::vector<component::VisualComponent*> added_;
        std::vector<component::VisualComponent*> removed_;
        std::vector<component::VisualComponent*> changed_;
    };
} // namespace soil::stage::scene::render

#endif // SOIL_STAGE_SCENE_RENDER_DEFAULT_CONTAINER_H
