#include "rotation_node.h"
#include <glm/gtc/matrix_transform.hpp>

namespace soil_samples::common {
    RotationNode::RotationNode(Node* parent, const float initialRotation, float velocity) :
        Node(parent, Type::Transform, {}), velocity_(velocity) {
        auto transform = Object3d::GetLocalTransform();
        transform = glm::rotate(transform, initialRotation, glm::vec3(0.F, 0.F, 1.F));
        Node::SetLocalTransform(transform);
        SetUpdateType(UpdateType::Active);
    }

    void RotationNode::Update() {
        auto transform = Object3d::GetLocalTransform();
        transform = glm::rotate(transform, velocity_, glm::vec3(0.F, 0.F, 1.F));
        Node::SetLocalTransform(transform);
        Node::Update();
    }
} // namespace soil_samples::common
