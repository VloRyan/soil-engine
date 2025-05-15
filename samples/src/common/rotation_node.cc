#include "rotation_node.h"
#include <glm/gtc/matrix_transform.hpp>

namespace soil_samples::common {
    RotationNode::RotationNode(Node* parent, const float initialRotation) : Node(parent, Type::Transform, {}) {
        auto transform = Object3d::GetLocalTransform();
        transform = glm::rotate(transform, initialRotation, glm::vec3(0.F, 0.F, 1.F));
        Node::SetLocalTransform(transform);
        SetUpdateType(UpdateType::Active);
    }

    void RotationNode::Update() {
        auto transform = Object3d::GetLocalTransform();
        transform = glm::rotate(transform, 0.1F, glm::vec3(0.F, 0.F, 1.F));
        Node::SetLocalTransform(transform);
        Node::Update();
    }
} // namespace soil_samples::common
