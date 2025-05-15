#include "world/collision/frustum.h"

#include <stdexcept>

namespace soil::world::collision {
    Frustum::Frustum(const glm::mat4 &viewProjection) : planes_() {
        SetViewProjection(viewProjection);
    }

    void Frustum::SetViewProjection(const glm::mat4 &viewProjection) {
        // Extract frustum planes from matrix
        planes_[static_cast<int>(Planes::Right)] =
            glm::vec4(viewProjection[0][3] - viewProjection[0][0], viewProjection[1][3] - viewProjection[1][0],
                      viewProjection[2][3] - viewProjection[2][0], viewProjection[3][3] - viewProjection[3][0]);

        planes_[static_cast<int>(Planes::Left)] =
            glm::vec4(viewProjection[0][3] + viewProjection[0][0], viewProjection[1][3] + viewProjection[1][0],
                      viewProjection[2][3] + viewProjection[2][0], viewProjection[3][3] + viewProjection[3][0]);

        planes_[static_cast<int>(Planes::Bottom)] =
            glm::vec4(viewProjection[0][3] + viewProjection[0][1], viewProjection[1][3] + viewProjection[1][1],
                      viewProjection[2][3] + viewProjection[2][1], viewProjection[3][3] + viewProjection[3][1]);

        planes_[static_cast<int>(Planes::Top)] =
            glm::vec4(viewProjection[0][3] - viewProjection[0][1], viewProjection[1][3] - viewProjection[1][1],
                      viewProjection[2][3] - viewProjection[2][1], viewProjection[3][3] - viewProjection[3][1]);

        planes_[static_cast<int>(Planes::Far)] =
            glm::vec4(viewProjection[0][3] - viewProjection[0][2], viewProjection[1][3] - viewProjection[1][2],
                      viewProjection[2][3] - viewProjection[2][2], viewProjection[3][3] - viewProjection[3][2]);

        planes_[static_cast<int>(Planes::Near)] =
            glm::vec4(viewProjection[0][3] + viewProjection[0][2], viewProjection[1][3] + viewProjection[1][2],
                      viewProjection[2][3] + viewProjection[2][2], viewProjection[3][3] + viewProjection[3][2]);
        // Normalize them
        for (auto i = 0; i < PlanesCount; ++i) {
            planes_[i] = normalize(planes_[i]);
        }
    }

    bool Frustum::IsVisible(const std::vector<glm::vec4> &point, const int count) const {
        for (auto planeIndex = 0; planeIndex < PlanesCount; ++planeIndex) {
            for (auto pointIndex = 0; pointIndex < count; ++pointIndex) {
                if (const float dist = glm::dot(planes_[planeIndex], point[pointIndex]); dist >= 0.0F) {
                    return true;
                }
            }
        }
        return false;
    }

    bool Frustum::IsVisible(const std::vector<glm::vec3> &point, const int count) const {
        for (auto planeIndex = 0; planeIndex < PlanesCount; ++planeIndex) {
            bool inside = false;
            for (auto pointIndex = 0; pointIndex < count; ++pointIndex) {
                if (planes_[planeIndex].x * point[pointIndex].x + planes_[planeIndex].y * point[pointIndex].y +
                        planes_[planeIndex].z * point[pointIndex].z + planes_[planeIndex].w >
                    0) {
                    inside = true;
                    break;
                }
            }
            if (!inside) {
                return false;
            }
        }
        return true;
    }

    bool Frustum::IsVisible(const glm::vec3 &point) const {
        const auto plane = glm::vec4(point, 1.0F);
        for (auto i = 0; i < 6; ++i) {
            if (const float dist = glm::dot(planes_[i], plane); dist < 0.0F) {
                return false;
            }
        }
        return true;
    }

    bool Frustum::IsVisible(const glm::vec3 &point, const float ratio) const {
        const auto plane = glm::vec4(point, 1.0F);
        for (auto i = 0; i < 6; ++i) {
            if (const float dist = glm::dot(planes_[i], plane); dist < -ratio) {
                return false;
            }
        }
        return true;
    }

    bool Frustum::IsVisibleXZ(const glm::vec3 &point) const {
        const auto plane = glm::vec4(point, 1.0F);

        float dist = glm::dot(planes_[static_cast<int>(Planes::Left)], plane);
        if (dist < 0.0F) {
            return false;
        }
        dist = glm::dot(planes_[static_cast<int>(Planes::Right)], plane);
        if (dist < 0.0F) {
            return false;
        }
        dist = glm::dot(planes_[static_cast<int>(Planes::Far)], plane);
        if (dist < 0.0F) {
            return false;
        }
        dist = glm::dot(planes_[static_cast<int>(Planes::Near)], plane);
        return dist >= 0.0F;
    }

    bool Frustum::IsVisibleXZ(const glm::vec3 &point, const float ratio) const {
        const auto plane = glm::vec4(point, 1.0F);

        float dist = glm::dot(planes_[static_cast<int>(Planes::Left)], plane);
        if (dist < -ratio) {
            return false;
        }
        dist = glm::dot(planes_[static_cast<int>(Planes::Right)], plane);
        if (dist < -ratio) {
            return false;
        }
        dist = glm::dot(planes_[static_cast<int>(Planes::Far)], plane);
        if (dist < -ratio) {
            return false;
        }
        dist = glm::dot(planes_[static_cast<int>(Planes::Near)], plane);
        return dist >= -ratio;
    }

    glm::vec4 Frustum::GetPlane(Planes plane) const {
        return planes_[static_cast<int>(plane)];
    }

    glm::vec3 Frustum::GetPoint(Points point) const {
        glm::vec4 nearPlane = GetPlane(Planes::Near);
        glm::vec4 farPlane = GetPlane(Planes::Far);
        glm::vec4 topPlane = GetPlane(Planes::Top);
        glm::vec4 bottomPlane = GetPlane(Planes::Bottom);
        glm::vec4 leftPlane = GetPlane(Planes::Left);
        glm::vec4 rightPlane = GetPlane(Planes::Right);
        switch (point) {
        case NearTopLeft:
            return intersectionPoint(nearPlane, leftPlane, topPlane);
        case NearBottomLeft:
            return intersectionPoint(nearPlane, leftPlane, bottomPlane);
        case NearTopRight:
            return intersectionPoint(nearPlane, rightPlane, topPlane);
        case NearBottomRight:
            return intersectionPoint(nearPlane, rightPlane, bottomPlane);
        case FarTopLeft:
            return intersectionPoint(farPlane, leftPlane, topPlane);
        case FarBottomLeft:
            return intersectionPoint(farPlane, leftPlane, bottomPlane);
        case FarTopRight:
            return intersectionPoint(farPlane, rightPlane, topPlane);
        case FarBottomRight:
            return intersectionPoint(farPlane, rightPlane, bottomPlane);

        default:
            throw std::runtime_error("unknown Point");
        }
    }

    glm::vec4 Frustum::normalize(const glm::vec4 plane) {
        // Normalizing the planes.

        const glm::vec3 normal(plane.x, plane.y, plane.z);
        const float length = glm::length(normal);

        return plane / length; // Notice the negation. I don't know why I needed that!!
    }

    glm::vec3 Frustum::intersectionPoint(glm::vec4 planeA, glm::vec4 planeB, glm::vec4 planeC) {
        // Formula used
        //                d1 ( N2 * N3 ) + d2 ( N3 * N1 ) + d3 ( N1 * N2 )
        // P =   ---------------------------------------------------------------------
        //                             N1 . ( N2 * N3 )
        //
        // Note: N refers to the normal, d refers to the displacement. '.' means dot
        // product. '*' means cross product

        glm::vec3 v1;
        glm::vec3 v2;
        glm::vec3 v3;
        glm::vec3 cross = glm::cross(glm::vec3(planeB), glm::vec3(planeC));
        float f = -glm::dot(glm::vec3(planeA), cross);

        cross = glm::cross(glm::vec3(planeB), glm::vec3(planeC));
        v1 = cross * glm::vec3(planeA.w);

        cross = glm::cross(glm::vec3(planeC), glm::vec3(planeA));
        v2 = cross * glm::vec3(planeB.w);

        cross = glm::cross(glm::vec3(planeA), glm::vec3(planeB));
        v3 = cross * glm::vec3(planeC.w);

        glm::vec3 vec = v1 + v2 + v3;

        return vec / glm::vec3(f);
    }
} // namespace soil::world::collision
