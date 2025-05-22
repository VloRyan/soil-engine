#include "../../../include/world/volume/frustum.h"

#include <stdexcept>

namespace soil::world::volume {
    Frustum::Frustum(const glm::mat4 &projectionView) : planes_() {
        SetProjectionView(projectionView);
    }

    void Frustum::SetProjectionView(const glm::mat4 &pv) {
        // Extract frustum planes from matrix
        for (int i = 0; i < 4; i++) {
            planes_[static_cast<int>(Planes::Left)][i] = pv[i][3] + pv[i][0];
            planes_[static_cast<int>(Planes::Right)][i] = pv[i][3] - pv[i][0];
            planes_[static_cast<int>(Planes::Top)][i] = pv[i][3] - pv[i][1];
            planes_[static_cast<int>(Planes::Bottom)][i] = pv[i][3] + pv[i][1];
            planes_[static_cast<int>(Planes::Near)][i] = pv[i][3] + pv[i][2];
            planes_[static_cast<int>(Planes::Far)][i] = pv[i][3] - pv[i][2];
        }
        // Normalize them
        for (auto i = 0; i < static_cast<int>(Planes::COUNT); ++i) {
            const auto mag = glm::length(glm::vec3(planes_[i]));
            planes_[i] = planes_[i] / mag;
        }
    }

    bool Frustum::IsVisible(const std::vector<glm::vec3> &point, const int count) const {
        for (auto pointIndex = 0; pointIndex < count; ++pointIndex) {
            if (IsVisible(point[pointIndex])) {
                return true;
            }
        }
        return false;
    }

    bool Frustum::IsVisible(const glm::vec3 &point) const {
        const auto plane = glm::vec4(point, 1.0F);
        for (auto planeIndex = 0; planeIndex < static_cast<int>(Planes::COUNT); ++planeIndex) {
            const float dist = glm::dot(planes_[planeIndex], plane);
            if (dist < 0.F) {
                return false;
            }
        }
        return true;
    }

    bool Frustum::IsVisible(const glm::vec3 &point, const float ratio) const {
        const auto plane = glm::vec4(point, 1.0F);
        for (auto i = 0; i < static_cast<int>(Planes::COUNT); ++i) {
            if (const float dist = glm::dot(planes_[i], plane); dist < -ratio) {
                return false;
            }
        }
        return true;
    }

    bool Frustum::IntersectBox(const glm::vec3 &min, const glm::vec3 &max) const {
        if (!IsVisible(Planes::Left, min, max) || !IsVisible(Planes::Right, min, max) ||
            !IsVisible(Planes::Top, min, max) || !IsVisible(Planes::Bottom, min, max) ||
            !IsVisible(Planes::Far, min, max) || !IsVisible(Planes::Near, min, max)) {
            return false;
        }
        return true;
    }

    bool Frustum::IsVisible(Planes plane, const glm::vec3 &min, const glm::vec3 &max) const {
        const glm::vec4 &g = planes_[static_cast<int>(plane)];
        if (glm::dot(g, glm::vec4(min.x, min.y, min.z, 1.F)) < 0.F &&
            glm::dot(g, glm::vec4(max.x, min.y, min.z, 1.F)) < 0.F &&
            glm::dot(g, glm::vec4(min.x, max.y, min.z, 1.F)) < 0.F &&
            glm::dot(g, glm::vec4(max.x, max.y, min.z, 1.F)) < 0.F &&
            glm::dot(g, glm::vec4(min.x, min.y, max.z, 1.F)) < 0.F &&
            glm::dot(g, glm::vec4(max.x, min.y, max.z, 1.F)) < 0.F &&
            glm::dot(g, glm::vec4(min.x, max.y, max.z, 1.F)) < 0.F &&
            glm::dot(g, glm::vec4(max.x, max.y, max.z, 1.F)) < 0.F) {
            // Not visible - all returned negative
            return false;
        }
        // Potentially visible

        // check frustum outside/inside box
        for (int i = 0; i < static_cast<int>(Points::COUNT); i++) {
            const auto point = GetPoint(static_cast<Points>(i));
            if (point.x > max.x && //
                point.x < min.x && //
                point.y > max.y && //
                point.y < min.y && //
                point.z > max.z && //
                point.z < min.z) {
                return false;
            }
        }
        return true;
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
        case Points::NearTopLeft:
            return intersectionPoint(nearPlane, leftPlane, topPlane);
        case Points::NearBottomLeft:
            return intersectionPoint(nearPlane, leftPlane, bottomPlane);
        case Points::NearTopRight:
            return intersectionPoint(nearPlane, rightPlane, topPlane);
        case Points::NearBottomRight:
            return intersectionPoint(nearPlane, rightPlane, bottomPlane);
        case Points::FarTopLeft:
            return intersectionPoint(farPlane, leftPlane, topPlane);
        case Points::FarBottomLeft:
            return intersectionPoint(farPlane, leftPlane, bottomPlane);
        case Points::FarTopRight:
            return intersectionPoint(farPlane, rightPlane, topPlane);
        case Points::FarBottomRight:
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
} // namespace soil::world::volume
