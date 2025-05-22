#ifndef SOIL_WORLD_VOLUME_H
#define SOIL_WORLD_VOLUME_H

#include "glm/glm.hpp"

namespace soil::world::volume {
    struct IntersectionResult {
        bool Intersecting = false;
        glm::vec3 IntersectionPoint;
    };

    class Volume {
    public:
        virtual ~Volume() = default;

        [[nodiscard]] virtual bool IsInside(const glm::vec3& min, const glm::vec3& max) const = 0;

        [[nodiscard]] virtual bool IsInsideXZ(const glm::vec3& min, const glm::vec3& max) const = 0;

        [[nodiscard]] virtual bool Contains(const glm::vec3& point) const = 0;
        [[nodiscard]] virtual bool ContainsXZ(const glm::vec3& point) const = 0;

        [[nodiscard]] virtual IntersectionResult IntersectsRay(const glm::vec3& start, const glm::vec3& dir) const = 0;
        [[nodiscard]] virtual IntersectionResult IntersectsRayXZ(const glm::vec3& start,
                                                                 const glm::vec3& dir) const = 0;

        virtual void SetPosition(const glm::vec3& position) = 0;

    protected:
        explicit Volume() = default;
    };
} // namespace soil::world::volume
#endif // SOIL_WORLD_VOLUME_H
