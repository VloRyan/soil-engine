#include "stage/scene/viewer/perspective.h"
#include "engine.h"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>

namespace stage::scene::viewer {
Perspective::Perspective(scene::Node *parent, const glm::ivec2 resolution)
    : Node(parent, {ReceiverType::Window}), updateVelocity_(false),
      moveSpeed_(), direction_({0.0F, 0.0F, -1.0F}), right_({1.0F, 0.0F, 0.0F}),
      up_({0.0F, 1.0F, 0.0F}), rotate_(0.0F), velocity_(0.0F), fovY_(60.0F),
      nearZ_(0.1F), farZ_(1000.0F), perspective_(0.0F), view_(1.0F),
      frustum_(new world::collision::Frustum(glm::mat4(1.0F))) {
  SetRotate(rotate_);
  pipeline_ = new video::render::Pipeline("Perspective_Pipeline");
  pipeline_->Reset(false, resolution);
  pipeline_->Print();
}

Perspective::~Perspective() { delete frustum_; }

glm::mat4 Perspective::GetProjectionMatrix() const { return perspective_; }

glm::mat4 Perspective::GetViewMatrix() const { return view_; }

void Perspective::SetPerspective(const float fovY, const float aspect,
                                 const float nearZ, const float farZ) {
  fovY_ = fovY;
  nearZ_ = nearZ;
  farZ_ = farZ;
  perspective_ = glm::perspective(glm::radians(fovY_), aspect, nearZ_, farZ_);
  viewChanged_ = true;
}

void Perspective::Look(const glm::vec3 center, const glm::vec3 up) {
  view_ = glm::lookAt(GetWorldPosition(), center, up);
  glm::mat4 invTransform = inverse(view_);
  this->right_ = glm::vec3(invTransform[0]);
  this->up_ = glm::vec3(invTransform[1]);
  this->direction_ =
      glm::vec3(invTransform[2]) * glm::vec3(-1.0F); // direction is inverse
  frustum_->SetViewProjection(GetProjectionMatrix() * GetViewMatrix());
  viewChanged_ = false;
}

void Perspective::Move(const glm::vec3 move) {
  auto localTransform = GetLocalTransform();
  localTransform[3] += glm::vec4(right_ * move[0], 0);
  localTransform[3] += glm::vec4(up_ * move[1], 0);
  localTransform[3] += glm::vec4(direction_ * move[2], 0);
  SetLocalTransform(localTransform);
  viewChanged_ = true;
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
    viewChanged_ = true;
    updateVelocity_ = false;
  }
  if (viewChanged_) {
    UpdateDirty();
  }
  Node::Update();
}

void Perspective::UpdateDirty() {
  Node::UpdateDirty();
  view_ = glm::lookAt(GetWorldPosition(), GetWorldPosition() + GetDirection(),
                      GetUp());
  glm::mat4 invTransform = inverse(view_);
  right_ = glm::vec3(invTransform[0]);
  up_ = glm::vec3(invTransform[1]);
  direction_ =
      glm::vec3(invTransform[2]) * glm::vec3(-1.0F); // direction is inverse
  viewChanged_ = false;
  frustum_->SetViewProjection(GetProjectionMatrix() * GetViewMatrix());
}

void Perspective::SetDirection(const glm::vec3 direction) {
  direction_ = direction;
  viewChanged_ = true;
}

void Perspective::SetPosition(const glm::vec3 pos) {
  auto localTransform = GetLocalTransform();
  localTransform[3] = glm::vec4(pos, 1.0F);
  SetLocalTransform(localTransform);
  viewChanged_ = true;
}

void Perspective::SetRight(const glm::vec3 right) {
  right_ = right;
  viewChanged_ = true;
}

void Perspective::SetUp(const glm::vec3 up) {
  up_ = up;
  viewChanged_ = true;
}

glm::vec3 Perspective::GetDirection() const { return direction_; }

glm::vec3 Perspective::GetUp() const { return up_; }

glm::vec3 Perspective::GetRight() const { return right_; }

glm::vec3 Perspective::GetRotate() const { return this->rotate_; }

void Perspective::SetRotate(const glm::vec3 rot) {
  rotate_ = rot;

  direction_ = glm::vec3(cos(rot[1]) * sin(rot[0]), sin(rot[1]),
                         cos(rot[1]) * cos(rot[0]));

  right_ = glm::vec3(std::sin(rot[0] - 3.14F / 2.0F), 0,
                     std::cos(rot[0] - 3.14F / 2.0F));
  up_ = glm::cross(right_, direction_);
  viewChanged_ = true;
}

world::collision::Frustum *Perspective::GetFrustum() { return frustum_; }

void Perspective::Handle(const engine::WindowEvent &event) {
  if (event.GetCause() == engine::WindowEvent::SizeChanged) {
    const float aspect = static_cast<float>(event.GetWindow()->GetSize().x) /
                         static_cast<float>(event.GetWindow()->GetSize().y);
    SetPerspective(fovY_, aspect, nearZ_, farZ_);
    if (pipeline_->GetOutputBuffer() != nullptr) {
      pipeline_->GetOutputBuffer()->Resize(event.GetWindow()->GetSize());
    }
  }
}
} // namespace stage::scene::viewer
