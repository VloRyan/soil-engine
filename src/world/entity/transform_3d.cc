#include "world/entity/transform_3d.h"

namespace soil::world::entity {
Transform3d::Transform3d() : matrix_(1.0F) {}

Transform3d::~Transform3d() = default;

glm::vec3 Transform3d::GetPosition() const { return {matrix_[3]}; }

void Transform3d::SetPosition(const glm::vec3& pos) {
  matrix_[3] = glm::vec4(pos, 1.0F);
}

glm::vec3 Transform3d::GetDirection() const { return matrix_[2]; }

void Transform3d::SetDirection(const glm::vec3& direction) {
  matrix_[2] = glm::vec4(direction, 1.0F);
}

glm::vec3 Transform3d::GetUp() const { return {matrix_[1]}; }

void Transform3d::SetUp(const glm::vec3& up) {
  matrix_[1] = glm::vec4(up, 1.0F);
}

glm::vec3 Transform3d::GetRight() const { return {matrix_[0]}; }

void Transform3d::SetRight(const glm::vec3& right) {
  matrix_[0] = glm::vec4(right, 1.0F);
}
void Transform3d::SetMatrix(const glm::mat4& transform) { matrix_ = transform; }
const glm::mat4& Transform3d::GetMatrix() const { return matrix_; }
}  // namespace soil::world::entity
