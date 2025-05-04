#include "stage/scene/viewer/ortho.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "window.h"

namespace stage::scene::viewer
{
    Ortho::Ortho(scene::Node *parent, const glm::ivec2 resolution, const float orthoHeight) :
        Node(parent, {ReceiverType::Window}), projection_(glm::mat4(1)),
        view_(glm::lookAt(glm::vec3(0.F), glm::vec3(0.0F, 0.0F, -1.0F), glm::vec3(0.0F, 1.0F, 0.0F))),
        direction_(0.0F, 0.0F, -1.0F), right_(1.0F, 0.0F, 0.0F), up_(0.0F, 1.0F, 0.0F), nearZ_(-0.1F), farZ_(1000.F),
        frustum_(new world::collision::Frustum(glm::mat4(1.0F))), orthoHeight_(orthoHeight)
    {
        Ortho::SetProjection({resolution.x, resolution.y, farZ_});
        pipeline_ = new video::render::Pipeline("GUI_Pipeline");
        pipeline_->Reset(false, resolution);
        pipeline_->Print();
    }

    void Ortho::SetProjection(const glm::vec3 size)
    {
        farZ_ = size.z;
        const float aspect = static_cast<float>(size.x) / static_cast<float>(size.y);
        switch (orthoType_)
        {
        case OrthoType::Full:
            projection_ = glm::ortho(0.F, size.x, 0.F, size.y, nearZ_, farZ_);
            break;
        case OrthoType::OrthoHeight:
            const float halfHeight = orthoHeight_ * 0.5F;
            const float halfWidth = halfHeight * aspect;
            projection_ = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearZ_, farZ_);
            break;
        }
        frustum_->SetViewProjection(GetProjectionMatrix() * GetViewMatrix());
    }

    void Ortho::Look(const glm::vec3 pos, const glm::vec3 center, const glm::vec3 up)
    {
        view_ = glm::lookAt(pos + GetWorldPosition(), center, up);
        glm::mat4 invTransform = inverse(view_);
        this->right_ = glm::vec3(invTransform[0]);
        this->up_ = glm::vec3(invTransform[1]);
        this->direction_ = glm::vec3(invTransform[2]) * glm::vec3(-1.0F); // direction is inverse

        SetPosition(pos);
        frustum_->SetViewProjection(GetProjectionMatrix() * GetViewMatrix());
    }

    glm::mat4 Ortho::GetViewMatrix() const { return view_; }

    glm::mat4 Ortho::GetProjectionMatrix() const { return projection_; }

    world::collision::Frustum *Ortho::GetFrustum() { return frustum_; }

    void Ortho::SetPosition(const glm::vec3 pos)
    {
        Node::SetPosition(pos);
        viewChanged_ = true;
    }

    void Ortho::Update()
    {
        Node::Update();
        if (viewChanged_)
        {
            UpdateDirty();
        }
    }
    OrthoType Ortho::GetOrthoType() const { return orthoType_; }

    void Ortho::SetOrthoType(const OrthoType ortho_type) { orthoType_ = ortho_type; }

    void Ortho::UpdateDirty()
    {
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

    void Ortho::Handle(const engine::WindowEvent &event)
    {
        if (event.GetCause() == engine::WindowEvent::SizeChanged)
        {
            SetProjection(glm::vec3(event.GetWindow()->GetSize().x, event.GetWindow()->GetSize().y, farZ_));
            if (pipeline_->GetOutputBuffer() != nullptr)
            {
                pipeline_->GetOutputBuffer()->Resize(event.GetWindow()->GetSize());
            }
        }
    }
} // namespace stage::scene::viewer
