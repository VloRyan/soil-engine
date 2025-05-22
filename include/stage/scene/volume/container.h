#ifndef SOIL_STAGE_SCENE_VOLUME_CONTAINER_H
#define SOIL_STAGE_SCENE_VOLUME_CONTAINER_H
#include <functional>


#include "feature.h"
#include "stage/scene/component/component.h"
#include "world/volume/container.h"

namespace soil::stage::scene::volume {
    class Container : public Feature {
    public:
        explicit Container(world::volume::Container* container);
        ~Container() override;
        void ComponentAdded(component::Component* component) override;
        void ComponentChanged(component::Component* component) override;
        void ComponentRemoved(component::Component* component) override;
        void Update() override;
        void ForEachAt(glm::vec3 pos, const std::function<bool(const component::BoundingVolume* volume)>& f) const;

        [[nodiscard]] const world::volume::Container* GetContainer() const;


    private:
        world::volume::Container* container_;
    };
} // namespace soil::stage::scene::volume

#endif // CONTAINER_H
