#include "stage/scene/viewer/perspective.h"

#include <cmath>

#include "glm/gtc/matrix_transform.hpp"
#include "window.h"

namespace soil::stage::scene::viewer {
    Perspective::Perspective() :
        updateVelocity_(false), moveSpeed_(), rotate_(0.0F), velocity_(0.0F), fovY_(60.0F), perspective_(0.0F),
        view_(1.0F) {
        nearZ_ = 0.1F;
        SetRotate(rotate_);
        Node::SetReceiverType(ReceiverType::Window, true);
    }

    glm::mat4 Perspective::GetProjectionMatrix() const {
        return perspective_;
    }

    glm::mat4 Perspective::GetViewMatrix() const {
        return view_;
    }

    void Perspective::Look(const glm::vec3 center, const glm::vec3 up) {
        view_ = glm::lookAt(GetWorldPosition(), center, up);
        glm::mat4 invTransform = inverse(view_);
        this->right_ = glm::vec3(invTransform[0]);
        this->up_ = glm::vec3(invTransform[1]);
        this->direction_ = glm::vec3(invTransform[2]) * glm::vec3(-1.0F); // direction is inverse
        frustum_->SetProjectionView(GetProjectionMatrix() * GetViewMatrix());
    }

    void Perspective::Move(const glm::vec3 move) {
        auto localTransform = GetLocalTransform();
        localTransform[3] += glm::vec4(right_ * move[0], 0);
        localTransform[3] += glm::vec4(up_ * move[1], 0);
        localTransform[3] += glm::vec4(direction_ * move[2], 0);
        SetLocalTransform(localTransform);
    }

    void Perspective::AddVelocity(glm::vec3 velocity, const bool relative) {
        if (relative) {
            velocity_ += right_ * velocity[0];
            velocity_ += up_ * velocity[1];
            velocity_ += direction_ * velocity[2];
        } else {
            velocity_ += velocity;
        }
        updateVelocity_ = true;
    }

    void Perspective::Update() {
        auto localTransform = GetLocalTransform();
        if (updateVelocity_) {
            localTransform[3] += glm::vec4(velocity_, 0);
            SetLocalTransform(localTransform);
            velocity_ = glm::vec3(0.0F);
            updateVelocity_ = false;
        }
        Node::Update();
    }

    void Perspective::UpdateDirty() {
        Node::UpdateDirty();
        if (IsDirtyImpact(DirtyImpact::Dependents)) {
            view_ = glm::lookAt(GetWorldPosition(), GetWorldPosition() + GetDirection(), GetUp());
            glm::mat4 invTransform = inverse(view_);
            right_ = glm::vec3(invTransform[0]);
            up_ = glm::vec3(invTransform[1]);
            direction_ = glm::vec3(invTransform[2]) * glm::vec3(-1.0F); // direction is inverse
            frustum_->SetProjectionView(GetProjectionMatrix() * GetViewMatrix());
        }
    }

    void Perspective::UpdateProjection(const glm::ivec2& size) {
        const float aspect = static_cast<float>(size.x) / static_cast<float>(size.y);
        perspective_ = glm::perspective(glm::radians(fovY_), aspect, nearZ_, farZ_);
    }

    void Perspective::SetDirection(const glm::vec3& direction) {
        direction_ = direction;
    }

    void Perspective::SetPosition(const glm::vec3& pos) {
        auto localTransform = GetLocalTransform();
        localTransform[3] = glm::vec4(pos, 1.0F);
        SetLocalTransform(localTransform);
    }

    void Perspective::SetRight(const glm::vec3& right) {
        right_ = right;
    }

    void Perspective::SetUp(const glm::vec3& up) {
        up_ = up;
    }

    glm::vec3 Perspective::GetDirection() const {
        return direction_;
    }

    glm::vec3 Perspective::GetUp() const {
        return up_;
    }

    glm::vec3 Perspective::GetRight() const {
        return right_;
    }

    glm::vec3 Perspective::GetRotate() const {
        return this->rotate_;
    }

    void Perspective::SetRotate(const glm::vec3 rot) {
        rotate_ = rot;

        direction_ = glm::vec3(cos(rot[1]) * sin(rot[0]), sin(rot[1]), cos(rot[1]) * cos(rot[0]));

        right_ = glm::vec3(std::sin(rot[0] - 3.14F / 2.0F), 0, std::cos(rot[0] - 3.14F / 2.0F));
        up_ = glm::cross(right_, direction_);
    }

} // namespace soil::stage::scene::viewer
