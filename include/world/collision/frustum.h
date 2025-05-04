#ifndef ENGINE_WORLD_COLLISION_FRUSTUM_H_
#define ENGINE_WORLD_COLLISION_FRUSTUM_H_

#include <array>
#include <vector>
#include <glm/glm.hpp>

namespace world::collision {
    constexpr int PlanesCount = 6;

    enum class Planes : std::uint8_t {
        Left = 0,
        Right,
        Top,
        Bottom,
        Far,
        Near
    };

    enum Points : std::uint8_t {
        NearTopLeft = 0,
        NearBottomLeft,
        NearBottomRight,
        NearTopRight,
        FarTopLeft,
        FarBottomLeft,
        FarBottomRight,
        FarTopRight
    };

    class Frustum {
    public:
        explicit Frustum(const glm::mat4 &viewProjection);

        virtual ~Frustum();

        void SetViewProjection(const glm::mat4 &viewProjection);

        [[nodiscard]] bool IsVisible(const std::vector<glm::vec4> &point, int count) const;

        [[nodiscard]] bool IsVisible(const std::vector<glm::vec3> &point, int count) const;

        [[nodiscard]] bool IsVisible(const glm::vec3 &point) const;

        [[nodiscard]] bool IsVisible(const glm::vec3 &point, float ratio) const;

        [[nodiscard]] bool IsVisibleXZ(const glm::vec3 &point) const;

        [[nodiscard]] bool IsVisibleXZ(const glm::vec3 &point, float ratio) const;

        [[nodiscard]] glm::vec4 GetPlane(Planes plane) const;

        [[nodiscard]] glm::vec3 GetPoint(Points point) const;

    private:
        static glm::vec4 normalize(glm::vec4 plane);

        [[nodiscard]] static glm::vec3 intersectionPoint(glm::vec4 planeA, glm::vec4 planeB, glm::vec4 planeC);

        // Planes are in format: normal(xyz), offset(w)
        std::array<glm::vec4, PlanesCount> planes_;
    };
} // world::collision

#endif //ENGINE_WORLD_COLLISION_FRUSTUM_H_
