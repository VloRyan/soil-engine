#include "world/entity/collision_object.h"
namespace soil::world::entity {
CollisionObject::CollisionObject(volume::Volume* volume,
                                 CollisionObject::ContactType contactType,
                                 const std::vector<ContactType>& contactTypes)
    : contactType_(contactType),
      velocity_(0.F),
      volume_(volume),
      onWorldUpdate_(nullptr),
      state_(State::Inactive),
      contactResponse_(nullptr) {
  for (const auto type : contactTypes) {
    SetCollideWith(type, true);
  }
}

void CollisionObject::Update(const glm::vec3& position,
                             const glm::vec3& velocity) {
  Object3d::SetPosition(position);
  velocity_ = velocity;
  if (onWorldUpdate_ != nullptr) {
    onWorldUpdate_();
  }
}

glm::vec3 CollisionObject::GetVelocity() const { return velocity_; }

void CollisionObject::SetVelocity(const glm::vec3& velocity) {
  velocity_ = velocity;
}

void CollisionObject::HandleCollision(const entity::CollisionObject* other) {
  if (contactResponse_ != nullptr) {
    contactResponse_(this, other);
  } else {
    velocity_ = glm::vec3(0.F);
  }
}
volume::Volume* CollisionObject::GetVolume() const { return volume_; }

bool CollisionObject::CanCollideWith(ContactType type) const {
  return collideWithTypes_[static_cast<std::int8_t>(type)];
}

void CollisionObject::SetCollideWith(ContactType type, const bool value) {
  collideWithTypes_[static_cast<std::int8_t>(type)] = value;
}

CollisionObject::ContactType CollisionObject::GetContactType() const {
  return contactType_;
}

bool CollisionObject::IsInside(const glm::vec3& min,
                               const glm::vec3& max) const {
  return volume_->IsInside(GetPosition(), min, max);
}

bool CollisionObject::IsInsideXZ(const glm::vec3& min,
                                 const glm::vec3& max) const {
  return volume_->IsInsideXZ(GetPosition(), min, max);
}

bool CollisionObject::IsInside(const glm::vec2& min,
                               const glm::vec2& max) const {
  return volume_->IsInside(GetPosition(), min, max);
}

bool CollisionObject::Contains(const glm::vec3& point) const {
  return volume_->Contains(GetPosition(), point);
}

bool CollisionObject::ContainsXZ(const glm::vec3& point) const {
  return volume_->ContainsXZ(GetPosition(), point);
}

volume::IntersectionResult CollisionObject::IntersectsRay(
    const glm::vec3& start, const glm::vec3& dir) const {
  return volume_->IntersectsRay(GetPosition(), start, dir);
}

volume::IntersectionResult CollisionObject::IntersectsRayXZ(
    const glm::vec3& start, const glm::vec3& dir) const {
  return volume_->IntersectsRayXZ(GetPosition(), start, dir);
}

bool CollisionObject::IntersectsCircle(const glm::vec2& circleCenter,
                                       const float radius) const {
  return volume_->IntersectsCircle(GetPosition(), circleCenter, radius);
}

std::vector<volume::Line> CollisionObject::GenerateLines() const {
  return volume_->GenerateLines(GetPosition());
}

void CollisionObject::SetPosition(const glm::vec3& position) {
  Object3d::SetPosition(position);
  if (onWorldUpdate_ != nullptr) {
    onWorldUpdate_();
  }
}

void CollisionObject::SetOnWorldUpdate(
    const std::function<void()>& onWorldUpdate) {
  onWorldUpdate_ = onWorldUpdate;
}
CollisionObject::State CollisionObject::GetState() const { return state_; }
void CollisionObject::SetState(CollisionObject::State state) { state_ = state; }
void CollisionObject::SetContactResponse(
    const std::function<void(entity::CollisionObject*,
                             const entity::CollisionObject*)>&
        contactResponse) {
  contactResponse_ = contactResponse;
}
}  // namespace soil::world::entity
