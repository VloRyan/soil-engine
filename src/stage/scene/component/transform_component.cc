#include "stage/scene/component/transform_component.h"

#include "stage/scene/node.h"

namespace soil::stage::scene::component {
TransformComponent::TransformComponent() : Component(Type::Transform) {}

void TransformComponent::UpdateTransform(const glm::mat4& parentTransform) {
  worldTransform_.SetMatrix(parentTransform * localTransform_.GetMatrix());
  Component::Update();
}

void TransformComponent::SetPosition(const glm::vec3& pos) {
  auto relPos =
      pos - worldTransform_.GetPosition() + localTransform_.GetPosition();
  if (relPos == localTransform_.GetPosition()) {
    return;
  }
  localTransform_.SetPosition(relPos);
  worldTransform_.SetPosition(pos);
  SignalChanged();
}

glm::vec3 TransformComponent::GetPosition() const {
  return worldTransform_.GetPosition();
}
void TransformComponent::SetLocalPosition(const glm::vec3& pos) {
  auto diff = pos - localTransform_.GetPosition();
  if (diff == glm::vec3(0.F)) {
    return;
  }
  localTransform_.SetPosition(pos);
  worldTransform_.SetPosition(worldTransform_.GetPosition() + diff);
  SignalChanged();
}
glm::vec3 TransformComponent::GetLocalPosition() const {
  return localTransform_.GetPosition();
}
glm::vec3 TransformComponent::GetDirection() const {
  return worldTransform_.GetDirection();
}

glm::vec3 TransformComponent::GetUp() const { return worldTransform_.GetUp(); }
glm::vec3 TransformComponent::GetRight() const {
  return worldTransform_.GetRight();
}
const glm::mat4& TransformComponent::GetMatrix() const {
  return worldTransform_.GetMatrix();
}
const glm::mat4& TransformComponent::GetLocalMatrix() const {
  return localTransform_.GetMatrix();
}
void TransformComponent::SetLocalMatrix(const glm::mat4& transform) {
  localTransform_.SetMatrix(transform);
  SignalChanged();
}
};  // namespace soil::stage::scene::component
