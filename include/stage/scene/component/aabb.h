#ifndef SOIL_STAGE_SCENE_COMPONENT_AABB_H_
#define SOIL_STAGE_SCENE_COMPONENT_AABB_H_

#include <array>
#include <bitset>
#include <glm/glm.hpp>
#include "bounding_volume.h"

namespace soil::stage::scene::component {
    class AABB final : public BoundingVolume {
    public:
        enum class Point : std::uint8_t {
            TOP_FRONT_LEFT = 0,
            TOP_BACK_LEFT,
            TOP_BACK_RIGHT,
            TOP_FRONT_RIGHT,
            BOTTOM_FRONT_LEFT,
            BOTTOM_BACK_LEFT,
            BOTTOM_BACK_RIGHT,
            BOTTOM_FRONT_RIGHT
        };

        explicit AABB(glm::vec3 dimension, glm::vec3 scale = glm::vec3(1.0F));

        ~AABB() override;

        AABB(const AABB &other) = delete;

        AABB(AABB &&other) noexcept = delete;

        AABB &operator=(const AABB &other) = delete;

        AABB &operator=(AABB &&other) noexcept = delete;

        void GetTopPlane(std::array<glm::vec4, 4> points) const;

        void GetBottomPlane(std::array<glm::vec4, 4> points) const;

        void GetAllPoints(std::array<glm::vec4, 8> points) const;

        void UpdateMatrix(const glm::mat4 &matrix) override;

        void SetScale(glm::vec3 scale) override;

        [[nodiscard]] glm::vec3 GetScale() override;

        void SetDimension(glm::vec3 dimension) override;

        [[nodiscard]] glm::vec3 GetPoint(Point point) const;

        [[nodiscard]] bool IsInsideXZ(glm::vec2 min, glm::vec2 max) const override;

        [[nodiscard]] bool ContainsXZ(glm::vec2 point) const override;

        [[nodiscard]] IntersectionResult IntersectsRay(glm::vec3 start, glm::vec3 dir) const override;

        [[nodiscard]] IntersectionResult IntersectsRay2d(glm::vec2 start, glm::vec2 dir) const override;

        void DrawDebugLines(IDebugDrawer *debugDrawer) override;

        void SetParent(Node *parent) override;

        [[nodiscard]] bool IsContactType(ContactType contactType) const override;

        void SetContactType(ContactType type, bool value) override;

    private:
        void UpdateWorldPoints(const glm::vec3 &position);

        std::array<glm::vec3, 8> points_;
        std::array<glm::vec4, 8> worldPoints_;
        glm::vec3 scale_;
        glm::vec3 minPoint_;
        IDebugDrawer *debugDrawer_;
        int firstLineId;
        std::bitset<3> contactTypes_;

    public:
        [[nodiscard]] const glm::vec3 &GetMinPoint() const;

        [[nodiscard]] const glm::vec3 &GetMaxPoint() const;

    private:
        glm::vec3 maxPoint_;
    };
} // namespace soil::stage::scene::component

#endif // SOIL_STAGE_SCENE_COMPONENT_AABB_H_
