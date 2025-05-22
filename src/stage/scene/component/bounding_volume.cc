#include "stage/scene/component/bounding_volume.h"

#include "stage/scene/node.h"

namespace soil::stage::scene::component {
    BoundingVolume::BoundingVolume(Volume* volume, const std::vector<ContactType>& contactTypes) :
        Component(Type::BoundingVolume), volume_(volume) {
        for (const auto contactType : contactTypes) {
            BoundingVolume::SetContactType(contactType, true);
        }
    }

    void BoundingVolume::UpdateMatrix(const glm::mat4& matrix) {
        Component::UpdateMatrix(matrix);
        volume_->SetPosition(matrix[3]);
    }

    bool BoundingVolume::IsContactType(ContactType type) const {
        return contactTypes_[static_cast<int>(type)];
    }

    void BoundingVolume::SetContactType(ContactType type, const bool value) {
        contactTypes_[static_cast<int>(type)] = value;
    }

    bool BoundingVolume::IsInside(const glm::vec3& min, const glm::vec3& max) const {
        return volume_->IsInside(min, max);
    }

    bool BoundingVolume::IsInsideXZ(const glm::vec3& min, const glm::vec3& max) const {
        return volume_->IsInsideXZ(min, max);
    }

    bool BoundingVolume::Contains(const glm::vec3& point) const {
        return volume_->Contains(point);
    }

    bool BoundingVolume::ContainsXZ(const glm::vec3& point) const {
        return volume_->ContainsXZ(point);
    }

    world::volume::IntersectionResult BoundingVolume::IntersectsRay(const glm::vec3& start,
                                                                    const glm::vec3& dir) const {
        return volume_->IntersectsRay(start, dir);
    }

    world::volume::IntersectionResult BoundingVolume::IntersectsRayXZ(const glm::vec3& start,
                                                                      const glm::vec3& dir) const {
        return volume_->IntersectsRayXZ(start, dir);
    }

    void BoundingVolume::SetPosition(const glm::vec3& position) {
        volume_->SetPosition(position);
        SetDirty();
    }

    void BoundingVolume::SetParent(Node* parent) {
        Component::SetParent(parent);
        if (GetParent() == parent) {
            return;
        }
        if (GetParent() != nullptr) {
            volume_->SetPosition(GetParent()->GetWorldPosition());
        }
    }
} // namespace soil::stage::scene::component
