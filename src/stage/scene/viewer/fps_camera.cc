#include "stage/scene/viewer/fps_camera.h"

#include "engine.h"
#include "glm/gtc/matrix_transform.hpp"
#include <algorithm>
#include <cmath>

namespace stage::scene::viewer {
FPSCamera::FPSCamera(scene::Node *parent, const glm::ivec2 resolution)
    : Perspective(parent, resolution), mouseSpeed_(1.0F),
      mouseControlled_(true) {}

FPSCamera::~FPSCamera() = default;

void FPSCamera::Update() {
  if (mouseControlled_) {
    updateRotation();
  }
  Perspective::Update();
}

void FPSCamera::updateRotation() {
  rotate_[0] += mouseSpeed_ * cursorPositionDiff_.x;
  rotate_[1] += mouseSpeed_ * cursorPositionDiff_.y;
  rotate_[1] = std::min(rotate_[1], 1.55F);
  rotate_[1] = std::max(rotate_[1], -1.55F);
  SetRotate(rotate_);
  cursorPositionDiff_ = glm::vec2(0.0F);
}

void FPSCamera::Handle(const input::Event &event) {
  if (event.GetOrigin() == input::Event::Origin::MousePosition &&
      event.GetCause() == input::Event::Cause::StateChanges) {
    cursorPositionDiff_ = cursorPosition_ - event.GetCursorPos();
    cursorPosition_ = event.GetCursorPos();
  }
}
} // namespace stage::scene::viewer
