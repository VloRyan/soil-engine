#ifndef SOIL_STAGE_SCENE_COMPONENT_FEATURE_HPP
#define SOIL_STAGE_SCENE_COMPONENT_FEATURE_HPP
#include "component/component.h"

namespace soil::stage::scene {

    class Scene;

    class ComponentFeature {
    public:
        enum class UpdateType { AfterScene, Render };
        virtual ~ComponentFeature() = default;
        virtual void ComponentAdded(component::Component* component) = 0;
        virtual void ComponentChanged(component::Component* component) = 0;
        virtual void ComponentRemoved(component::Component* component) = 0;
        virtual void Update() = 0;
        virtual void OnAddedToScene(Scene* scene);

        [[nodiscard]] UpdateType GetUpdateType() const;

    protected:
        explicit ComponentFeature(UpdateType type = UpdateType::AfterScene);

    private:
        UpdateType updateType_;
    };
} // namespace soil::stage::scene
#endif // SOIL_STAGE_SCENE_COMPONENT_FEATURE_HPP
