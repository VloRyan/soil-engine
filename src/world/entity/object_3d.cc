#include "world/entity/object_3d.h"

namespace soil::world::entity {
Object3d::Object3d() : transform_(1.0F) {}

Object3d::~Object3d() = default;

glm::vec3 Object3d::GetPosition() const { return {transform_[3]}; }

void Object3d::SetPosition(const glm::vec3& pos) {
  transform_[3] = glm::vec4(pos, 1.0F);
}

glm::vec3 Object3d::GetDirection() const { return transform_[2]; }

void Object3d::SetDirection(const glm::vec3& direction) {
  transform_[2] = glm::vec4(direction, 1.0F);
}

glm::vec3 Object3d::GetUp() const { return {transform_[1]}; }

void Object3d::SetUp(const glm::vec3& up) {
  transform_[1] = glm::vec4(up, 1.0F);
}

glm::vec3 Object3d::GetRight() const { return {transform_[0]}; }

void Object3d::SetRight(const glm::vec3& right) {
  transform_[0] = glm::vec4(right, 1.0F);
}
void Object3d::SetTransform(const glm::mat4& transform) {
  transform_ = transform;
}
const glm::mat4& Object3d::GetTransform() const { return transform_; }

}  // namespace soil::world::entity
