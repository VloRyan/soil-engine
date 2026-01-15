#include "stage/scene/component/transform_component.h"

#include "stage/scene/node.h"

namespace soil::stage::scene::component {
TransformComponent::TransformComponent() : Component(Type::Transform) {}

void TransformComponent::UpdateTransform(const glm::mat4& parentTransform) {
  worldTransform_.SetMatrix(parentTransform * localTransform_.GetMatrix());
  Component::Update();
}

void TransformComponent::SetPosition(const glm::vec3& pos) {
  if (pos == GetPosition()) {
    return;
  }
  auto parentPos =
      worldTransform_.GetPosition() - localTransform_.GetPosition();
  localTransform_.SetPosition(pos - parentPos);
  worldTransform_.SetPosition(pos);
  SignalChanged(-1);
}

glm::vec3 TransformComponent::GetPosition() const {
  return worldTransform_.GetPosition();
}

void TransformComponent::SetLocalPosition(const glm::vec3& pos) {
  if (pos == GetLocalPosition()) {
    return;
  }
  auto parentPos =
      worldTransform_.GetPosition() - localTransform_.GetPosition();
  localTransform_.SetPosition(pos);
  worldTransform_.SetPosition(parentPos + pos);
  SignalChanged(-1);
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
  SignalChanged(-1);
}
};  // namespace soil::stage::scene::component
