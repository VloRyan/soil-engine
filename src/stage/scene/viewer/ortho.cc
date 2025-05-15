#include "stage/scene/viewer/ortho.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "window.h"

namespace soil::stage::scene::viewer {
    Ortho::Ortho(scene::Node *parent, const glm::ivec2 resolution) :
        Node(parent, {ReceiverType::Window}), projection_(glm::mat4(1)),
        view_(glm::lookAt(glm::vec3(0.F), glm::vec3(0.0F, 0.0F, -1.0F), glm::vec3(0.0F, 1.0F, 0.0F))),
        direction_(0.0F, 0.0F, -1.0F), right_(1.0F, 0.0F, 0.0F), up_(0.0F, 1.0F, 0.0F), nearZ_(-0.1F), farZ_(1000.F),
        frustum_(new world::collision::Frustum(glm::mat4(1.0F))), orthoSize_(10), orthoType_(OrthoType::Full) {
        SetProjection({resolution.x, resolution.y, farZ_});
    }

    void Ortho::SetProjection(const glm::vec3 size) {
        farZ_ = size.z;
        switch (orthoType_) {
        case OrthoType::Full:
            projection_ = glm::ortho(0.F, size.x, 0.F, size.y, nearZ_, farZ_);
            break;
        case OrthoType::OrthoHeight:
            float aspect = 0.F;
            float halfHeight = 0.F;
            float halfWidth = 0.F;
            if (size.x > size.y) {
                aspect = static_cast<float>(size.x) / static_cast<float>(size.y);
                halfHeight = orthoSize_ * 0.5F;
                halfWidth = halfHeight * aspect;
            } else {
                aspect = static_cast<float>(size.y) / static_cast<float>(size.x);
                halfWidth = orthoSize_ * 0.5F;
                halfHeight = halfWidth * aspect;
            }

            projection_ = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearZ_, farZ_);
            break;
        }
        frustum_->SetViewProjection(GetProjectionMatrix() * GetViewMatrix());
    }

    void Ortho::Look(const glm::vec3 pos, const glm::vec3 center, const glm::vec3 up) {
        view_ = glm::lookAt(pos + GetWorldPosition(), center, up);
        glm::mat4 invTransform = inverse(view_);
        this->right_ = glm::vec3(invTransform[0]);
        this->up_ = glm::vec3(invTransform[1]);
        this->direction_ = glm::vec3(invTransform[2]) * glm::vec3(-1.0F); // direction is inverse

        SetPosition(pos);
        frustum_->SetViewProjection(GetProjectionMatrix() * GetViewMatrix());
    }

    glm::mat4 Ortho::GetViewMatrix() const {
        return view_;
    }

    glm::mat4 Ortho::GetProjectionMatrix() const {
        return projection_;
    }

    world::collision::Frustum *Ortho::GetFrustum() {
        return frustum_;
    }

    void Ortho::SetPosition(const glm::vec3 pos) {
        Node::SetPosition(pos);
        viewChanged_ = true;
    }

    void Ortho::Update() {
        Node::Update();
        if (viewChanged_) {
            UpdateDirty();
        }
    }

    OrthoType Ortho::GetOrthoType() const {
        return orthoType_;
    }

    void Ortho::SetOrthoType(const OrthoType ortho_type) {
        orthoType_ = ortho_type;
    }

    void Ortho::UpdateDirty() {
        Node::UpdateDirty();
        auto const wPos = GetWorldPosition();
        view_ = glm::lookAt(wPos, wPos + direction_, glm::vec3(0.0F, 1.0F, 0.0F));
        glm::mat4 invTransform = inverse(view_);
        right_ = glm::vec3(invTransform[0]);
        up_ = glm::vec3(invTransform[1]);
        direction_ = glm::vec3(invTransform[2]) * glm::vec3(-1.0F); // direction is inverse
        frustum_->SetViewProjection(GetProjectionMatrix() * GetViewMatrix());
        viewChanged_ = false;
    }

    void Ortho::Handle(const WindowEvent &event) {
        if (event.GetCause() == WindowEvent::SizeChanged) {
            SetProjection(glm::vec3(event.GetWindow()->GetSize().x, event.GetWindow()->GetSize().y, farZ_));
        }
    }
} // namespace soil::stage::scene::viewer
