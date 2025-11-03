#include "stage/scene/viewer/node.h"

#include "stage/scene/scene.h"
#include "stage/stage.h"
#include "window.h"

namespace soil::stage::scene::viewer {
Node::Node()
    : scene::Node(Type::Viewer),
      direction_({0.F, 0.F, -1.F}),
      right_({1.F, 0.F, 0.F}),
      up_({0.F, 1.F, 0.F}),
      nearZ_(0.1F),
      farZ_(100.0F),
      windowSize_(0),
      frustum_(new soil::world::volume::Frustum(glm::mat4(1.0F))) {}

Node::~Node() { delete frustum_; }
void Node::OnStageChanged(soil::stage::Stage* stage,
                          soil::stage::Stage* prevStage) {
  scene::Node::OnStageChanged(stage, prevStage);
  if (prevStage != nullptr) {
    prevStage->RemoveEventHook(this);
  }
  if (stage != nullptr) {
    stage->AddEventHook(this);
  }
}
float Node::GetNearZ() const { return nearZ_; }

void Node::SetNearZ(const float nearZ) {
  nearZ_ = nearZ;
  UpdateProjection(windowSize_);
}

float Node::GetFarZ() const { return farZ_; }

void Node::SetFarZ(const float farZ) {
  farZ_ = farZ;
  UpdateProjection(windowSize_);
}

soil::world::volume::Frustum* Node::GetFrustum() { return frustum_; }
/*
glm::vec3 Node::GetDirection() const { return direction_; }

glm::vec3 Node::GetRight() const { return right_; }

glm::vec3 Node::GetUp() const { return up_; }
*/
void Node::OnEvent(const WindowEvent& event) {
  if (event.Cause == WindowEvent::SizeChanged) {
    windowSize_ = event.Window->GetSize();
    UpdateProjection(windowSize_);
  }
}

}  // namespace soil::stage::scene::viewer
