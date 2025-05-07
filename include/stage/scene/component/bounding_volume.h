#ifndef SOIL_STAGE_SCENE_COMPONENT_BOUNDING_VOLUME_H_
#define SOIL_STAGE_SCENE_COMPONENT_BOUNDING_VOLUME_H_

#include <glm/glm.hpp>
#include "component.h"
#include "stage/scene/debug_drawer.h"

namespace soil::stage::scene {
    class QuadTree;
}

namespace soil::stage::scene::component {
    struct IntersectionResult {
        bool Intersecting = false;
        glm::vec3 IntersectionPoint;
    };

    class BoundingVolume : public Component {
    public:
        enum class ContactType {
            View = 0,
            Player = 1,
            World = 2,
        };

        explicit BoundingVolume();

        ~BoundingVolume() override = default;

        virtual void SetScale(glm::vec3 scale) = 0;

        [[nodiscard]] virtual glm::vec3 GetScale() = 0;

        virtual void SetDimension(glm::vec3 dimension) = 0;

        [[nodiscard]] virtual bool IsInsideXZ(glm::vec2 min, glm::vec2 max) const = 0;

        [[nodiscard]] virtual bool ContainsXZ(glm::vec2 point) const = 0;

        [[nodiscard]] virtual IntersectionResult IntersectsRay(glm::vec3 start, glm::vec3 dir) const = 0;

        [[nodiscard]] virtual IntersectionResult IntersectsRay2d(glm::vec2 start, glm::vec2 dir) const = 0;

        virtual void DrawDebugLines(IDebugDrawer *debugDrawer) = 0;

        [[nodiscard]] virtual bool IsContactType(ContactType type) const = 0;

        virtual void SetContactType(ContactType type, bool value) = 0;
    };
} // namespace soil::stage::scene::component


#endif // SOIL_STAGE_SCENE_COMPONENT_BOUNDING_VOLUME_H_
