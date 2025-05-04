#include "world/entity/object_3d.h"

namespace world::entity
{
    Object3d::Object3d() : localTransform_(1.0F), worldTransform_(1.0F) {}

    Object3d::~Object3d() = default;

    glm::vec3 Object3d::GetPosition() const { return {localTransform_[3]}; }

    glm::vec3 Object3d::GetWorldPosition() const { return {worldTransform_[3]}; }

    void Object3d::SetPosition(const glm::vec3 pos)
    {
        localTransform_[3] = glm::vec4(pos, 1.0F);
        // worldTransform_[3] = glm::vec4(pos, 1.0F);
    }

    glm::vec3 Object3d::GetDirection() const { return localTransform_[2]; }

    void Object3d::SetDirection(const glm::vec3 direction) { localTransform_[2] = glm::vec4(direction, 1.0F); }

    glm::vec3 Object3d::GetUp() const { return {localTransform_[1]}; }

    void Object3d::SetUp(const glm::vec3 up) { localTransform_[1] = glm::vec4(up, 1.0F); }

    glm::vec3 Object3d::GetRight() const { return {localTransform_[0]}; }

    void Object3d::SetRight(const glm::vec3 right) { localTransform_[0] = glm::vec4(right, 1.0F); }

    void Object3d::SetLocalTransform(const glm::mat4 &Transform) { localTransform_ = Transform; }

    const glm::mat4 &Object3d::GetLocalTransform() const { return localTransform_; }

    void Object3d::ComputeWorldTransform(const glm::mat4 &parentWorldTransform)
    {
        worldTransform_ = parentWorldTransform * localTransform_;
    }

    const glm::mat4 &Object3d::GetWorldTransform() const { return worldTransform_; }
} // namespace world::entity
