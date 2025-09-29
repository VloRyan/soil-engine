#include "stage/scene/component/bounding_volume.h"

#include "stage/scene/node.h"

namespace soil::stage::scene::component {
    BoundingVolume::BoundingVolume(Volume* volume, const ContactType contactType,
                                   const std::vector<ContactType>& contactTypes) :
        Component(Type::BoundingVolume), volume_(volume), container_(nullptr), contactType_(contactType) {
        for (const auto type : contactTypes) {
            SetCollideWith(type, true);
        }
    }

    void BoundingVolume::UpdateTransform(const glm::mat4& matrix) {
        auto removed = false;
        if (container_ != nullptr) {
            removed = container_->Remove(this);
        }
        Component::UpdateTransform(matrix);
        volume_->SetPosition(matrix[3]);
        if (removed) {
            container_->Insert(this);
        }
        Component::UpdateTransform(matrix);
    }

    bool BoundingVolume::CanCollideWith(ContactType type) const {
        return collideWithTypes_[static_cast<std::int8_t>(type)];
    }

    void BoundingVolume::SetCollideWith(ContactType type, const bool value) {
        collideWithTypes_[static_cast<std::int8_t>(type)] = value;
    }

    bool BoundingVolume::IsInside(const glm::vec3& min, const glm::vec3& max) const {
        return volume_->IsInside(min, max);
    }

    bool BoundingVolume::IsInsideXZ(const glm::vec3& min, const glm::vec3& max) const {
        return volume_->IsInsideXZ(min, max);
    }

    bool BoundingVolume::IsInside(const glm::vec2& min, const glm::vec2& max) const {
        return volume_->IsInside(min, max);
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

    bool BoundingVolume::IntersectsCircle(const glm::vec2& circleCenter, const float radius) const {
        return volume_->IntersectsCircle(circleCenter, radius);
    }

    void BoundingVolume::SetPosition(const glm::vec3& position) {
        if (position == volume_->GetPosition()) {
            return;
        }
        volume_->SetPosition(position);
        SetDirty();
    }

    glm::vec3 BoundingVolume::GetPosition() const {
        return volume_->GetPosition();
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

    void BoundingVolume::SetContainer(world::volume::Container* container) {
        if (container_ == container) {
            return;
        }
        if (container_ != nullptr) {
            if (!container_->Remove(this)) {
                throw std::runtime_error("[BoundingVolume::SetContainer]: Failed to remove from container.");
            }
        }
        container_ = container;
    }

    world::volume::Container* BoundingVolume::GetContainer() const {
        return container_;
    }

    std::vector<world::volume::Line> BoundingVolume::GenerateLines() const {
        return volume_->GenerateLines();
    }

    BoundingVolume::ContactType BoundingVolume::GetContactType() const {
        return contactType_;
    }
} // namespace soil::stage::scene::component
