#include "world/volume/aabb.h"
#include <algorithm>
#include <bits/ranges_algobase.h>
#include "world/intersection.h"

#include "stage/scene/node.h"

namespace soil::world::volume {
    AABB::AABB(const glm::vec3 dimension) : points_(), worldPoints_(), minPoint_(0.F), maxPoint_(0.F) {
        SetDimension(dimension);
    }

    AABB::~AABB() = default;

    void AABB::UpdateWorldPoints(const glm::vec3& position) {
        glm::vec3 min(std::numeric_limits<float>::max());
        glm::vec3 max(std::numeric_limits<float>::lowest());
        for (const auto point : points_) {
            glm::vec3 worldPoint = point;
            for (auto j = 0; j < 3; j++) {
                min[j] = std::min(min[j], worldPoint[j]);
                max[j] = std::max(max[j], worldPoint[j]);
            }
        }
        minPoint_ = min + position;
        maxPoint_ = max + position;

        worldPoints_[static_cast<int>(Point::TOP_FRONT_LEFT)] = glm::vec4(minPoint_.x, maxPoint_.y, maxPoint_.z, 1.0F);
        worldPoints_[static_cast<int>(Point::TOP_BACK_LEFT)] = glm::vec4(minPoint_.x, maxPoint_.y, minPoint_.z, 1.0F);
        worldPoints_[static_cast<int>(Point::TOP_BACK_RIGHT)] = glm::vec4(maxPoint_.x, maxPoint_.y, minPoint_.z, 1.0F);
        worldPoints_[static_cast<int>(Point::TOP_FRONT_RIGHT)] = glm::vec4(maxPoint_.x, maxPoint_.y, maxPoint_.z, 1.0F);

        worldPoints_[static_cast<int>(Point::BOTTOM_FRONT_LEFT)] =
            glm::vec4(minPoint_.x, minPoint_.y, maxPoint_.z, 1.0F);
        worldPoints_[static_cast<int>(Point::BOTTOM_BACK_LEFT)] =
            glm::vec4(minPoint_.x, minPoint_.y, minPoint_.z, 1.0F);
        worldPoints_[static_cast<int>(Point::BOTTOM_BACK_RIGHT)] =
            glm::vec4(maxPoint_.x, minPoint_.y, minPoint_.z, 1.0F);
        worldPoints_[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)] =
            glm::vec4(maxPoint_.x, minPoint_.y, maxPoint_.z, 1.0F);
    }

    void AABB::GetTopPlane(std::array<glm::vec4, 4> points) const {
        for (int i = 0; i < 4; i++) {
            points[i] = worldPoints_[i];
        }
    }

    void AABB::GetBottomPlane(std::array<glm::vec4, 4> points) const {
        for (uint i = 0; i < 4; i++) {
            points[i] = worldPoints_[4 + i];
        }
    }

    void AABB::GetAllPoints(std::array<glm::vec4, 8> points) const {
        std::ranges::copy(worldPoints_, points.begin());
    }

    void AABB::SetDimension(const glm::vec3 dimension) {
        const auto halfDim = dimension * glm::vec3(0.5F);
        points_[static_cast<int>(Point::TOP_FRONT_LEFT)] = glm::vec3(-halfDim.x, halfDim.y, halfDim.z);
        points_[static_cast<int>(Point::TOP_BACK_LEFT)] = glm::vec3(-halfDim.x, halfDim.y, -halfDim.z);
        points_[static_cast<int>(Point::TOP_BACK_RIGHT)] = glm::vec3(halfDim.x, halfDim.y, -halfDim.z);
        points_[static_cast<int>(Point::TOP_FRONT_RIGHT)] = glm::vec3(halfDim.x, halfDim.y, halfDim.z);

        points_[static_cast<int>(Point::BOTTOM_FRONT_LEFT)] = glm::vec3(-halfDim.x, -halfDim.y, halfDim.z);
        points_[static_cast<int>(Point::BOTTOM_BACK_LEFT)] = glm::vec3(-halfDim.x, -halfDim.y, -halfDim.z);
        points_[static_cast<int>(Point::BOTTOM_BACK_RIGHT)] = glm::vec3(halfDim.x, -halfDim.y, -halfDim.z);
        points_[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)] = glm::vec3(halfDim.x, -halfDim.y, halfDim.z);
    }

    void AABB::SetPosition(const glm::vec3& position) {
        UpdateWorldPoints(position);
    }

    glm::vec3 AABB::GetPosition() const {
        return maxPoint_ - minPoint_;
    }

    glm::vec3 AABB::GetPoint(const Point point) const {
        return worldPoints_[static_cast<int>(point)];
    }

    const glm::vec3& AABB::GetMinPoint() const {
        return minPoint_;
    }

    const glm::vec3& AABB::GetMaxPoint() const {
        return maxPoint_;
    }

    bool AABB::IsInside(const glm::vec3& min, const glm::vec3& max) const {
        for (auto i = 0; i < 3; i++) {
            if (minPoint_[i] > max[i] || maxPoint_[i] < min[i]) {
                return false;
            }
        }
        return true;
    }

    bool AABB::IsInsideXZ(const glm::vec3& min, const glm::vec3& max) const {
        if (minPoint_.x > max.x || maxPoint_.x < min.x) {
            return false;
        }
        if (minPoint_.z > max.z || maxPoint_.z < min.z) {
            return false;
        }
        return true;
    }

    bool AABB::IsInside(const glm::vec2& min, const glm::vec2& max) const {
        if (minPoint_.x > max.x || //
            maxPoint_.x < min.x || //
            minPoint_.z > max.y || //
            maxPoint_.z < min.y) {
            return false;
        }
        return true;
    }

    bool AABB::Contains(const glm::vec3& point) const {
        for (auto i = 0; i < 3; i++) {
            if (point[i] < minPoint_[i] || point[i] > maxPoint_[i]) {
                return false;
            }
        }
        return true;
    }

    bool AABB::ContainsXZ(const glm::vec3& point) const {
        if (point.x < minPoint_.x || point.x > maxPoint_.x) {
            return false;
        }
        if (point.z < minPoint_.z || point.z > maxPoint_.z) {
            return false;
        }
        return true;
    }

    IntersectionResult AABB::IntersectsRay(const glm::vec3& start, const glm::vec3& dir) const {
        constexpr auto NUMDIM = 3;
        constexpr auto RIGHT = 0;
        constexpr auto LEFT = 1;
        constexpr auto MIDDLE = 2;

        glm::vec3 hitPoint;

        bool inside = true;
        char quadrant[NUMDIM];
        int i;
        float maxT[NUMDIM];
        float candidatePlane[NUMDIM];

        // Find candidate planes; this loop can be avoided if rays cast all from the eye(assume perspective view)
        for (i = 0; i < NUMDIM; i++) {
            if (start[i] < minPoint_[i]) {
                quadrant[i] = LEFT;
                candidatePlane[i] = minPoint_[i];
                inside = false;
            } else if (start[i] > maxPoint_[i]) {
                quadrant[i] = RIGHT;
                candidatePlane[i] = maxPoint_[i];
                inside = false;
            } else {
                quadrant[i] = MIDDLE;
            }
        }

        // Ray origin inside bounding box
        if (inside) {
            return IntersectionResult{.Intersecting = true, .IntersectionPoint = start};
        }

        // Calculate T distances to candidate planes */
        for (i = 0; i < NUMDIM; i++) {
            if (quadrant[i] != MIDDLE && dir[i] != 0.) {
                maxT[i] = (candidatePlane[i] - start[i]) / dir[i];
            } else {
                maxT[i] = -1.;
            }
        }

        // Get largest of the maxT's for final choice of intersection */
        int whichPlane = 0;
        for (i = 1; i < NUMDIM; i++) {
            if (maxT[whichPlane] < maxT[i]) {
                whichPlane = i;
            }
        }

        // Check final candidate actually inside box
        if (maxT[whichPlane] < 0.) {
            return IntersectionResult{.Intersecting = false, .IntersectionPoint = glm::vec3(0, 0, 0)};
        }
        for (i = 0; i < NUMDIM; i++) {
            if (whichPlane != i) {
                hitPoint[i] = start[i] + maxT[whichPlane] * dir[i];
                if (hitPoint[i] < minPoint_[i] || hitPoint[i] > maxPoint_[i]) {
                    return IntersectionResult{.Intersecting = false, .IntersectionPoint = glm::vec3(0, 0, 0)};
                }
            } else {
                hitPoint[i] = candidatePlane[i];
            }
        }
        return IntersectionResult{.Intersecting = true, .IntersectionPoint = hitPoint}; // ray hits box
    }

    IntersectionResult AABB::IntersectsRayXZ(const glm::vec3& start, const glm::vec3& dir) const {
        return IntersectsRay({start.x, 0.F, start.z}, {dir.x, 0.F, dir.z});
    }

    bool AABB::IntersectsCircle(const glm::vec2& circleCenter, const float radius) const {
        return Intersection::Test(
            Intersection::Box2d{
                .Min = glm::vec2(minPoint_.x, minPoint_.z),
                .Max = glm::vec2(maxPoint_.x, maxPoint_.z),
            },
            Intersection::Circle2d{
                .Center = circleCenter,
                .Radius = radius,
            });
        /*const auto halfDim = glm::vec2(maxPoint_.x - minPoint_.x, maxPoint_.z - minPoint_.z) * glm::vec2(0.5F);
        const auto center = glm::vec2(minPoint_.x, minPoint_.z) + halfDim;
        const auto dist = glm::abs(circleCenter - center);
        if (dist.x <= radius && dist.y <= radius) {
            return true;
        }
        const auto dir = glm::normalize(center - circleCenter);
        const auto maxPoint = dir * radius + circleCenter;
        return ContainsXZ(glm::vec3(maxPoint.x, 0.F, maxPoint.y));*/
    }

    std::vector<Line> AABB::GenerateLines() const {
        return {
            {.Start = points_[static_cast<int>(Point::TOP_BACK_LEFT)],
             .End = points_[static_cast<int>(Point::TOP_BACK_RIGHT)]},
            {.Start = points_[static_cast<int>(Point::TOP_BACK_RIGHT)],
             .End = points_[static_cast<int>(Point::TOP_FRONT_RIGHT)]},
            {.Start = points_[static_cast<int>(Point::TOP_FRONT_RIGHT)],
             .End = points_[static_cast<int>(Point::TOP_FRONT_LEFT)]},
            {.Start = points_[static_cast<int>(Point::TOP_FRONT_LEFT)],
             .End = points_[static_cast<int>(Point::TOP_BACK_LEFT)]},

            {.Start = points_[static_cast<int>(Point::BOTTOM_BACK_LEFT)],
             .End = points_[static_cast<int>(Point::BOTTOM_BACK_RIGHT)]},
            {.Start = points_[static_cast<int>(Point::BOTTOM_BACK_RIGHT)],
             .End = points_[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)]},
            {.Start = points_[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)],
             .End = points_[static_cast<int>(Point::BOTTOM_FRONT_LEFT)]},
            {.Start = points_[static_cast<int>(Point::BOTTOM_FRONT_LEFT)],
             .End = points_[static_cast<int>(Point::BOTTOM_BACK_LEFT)]},

            {.Start = points_[static_cast<int>(Point::BOTTOM_BACK_LEFT)],
             .End = points_[static_cast<int>(Point::TOP_BACK_LEFT)]},
            {.Start = points_[static_cast<int>(Point::BOTTOM_BACK_RIGHT)],
             .End = points_[static_cast<int>(Point::TOP_BACK_RIGHT)]},
            {.Start = points_[static_cast<int>(Point::BOTTOM_FRONT_RIGHT)],
             .End = points_[static_cast<int>(Point::TOP_FRONT_RIGHT)]},
            {.Start = points_[static_cast<int>(Point::BOTTOM_FRONT_LEFT)],
             .End = points_[static_cast<int>(Point::TOP_FRONT_LEFT)]},

        };
    }
} // namespace soil::world::volume
