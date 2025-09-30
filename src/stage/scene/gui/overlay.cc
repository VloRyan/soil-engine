#include "stage/scene/gui/overlay.h"

#include "stage/scene/gui/root.h"

namespace soil::stage::scene::gui {
Overlay::Overlay(Root* root, const bool positionRelativeToRoot)
    : root_(root), positionRelativeToRoot_(positionRelativeToRoot) {
  Node::SetPosition(glm::vec3(0.F));
}

Rectangle* Overlay::GetParentRect() const {
  if (root_ == nullptr) {
    return Rectangle::GetParentRect();
  }
  return root_;
}

bool Overlay::IsPositionRelativeToRoot() const {
  return positionRelativeToRoot_;
}

void Overlay::SetPositionRelativeToRoot(const bool positionRelativeToRoot) {
  this->positionRelativeToRoot_ = positionRelativeToRoot;
}

void Overlay::ComputeWorldTransform(const glm::mat4& parentWorldTransform) {
  auto topLevel = parentWorldTransform;
  if (positionRelativeToRoot_) {
    topLevel[3] = glm::vec4(0.F, 0.F, -10.F, 1.F);
  } else {
    topLevel[3].z = -10.F;
  }
  Rectangle::ComputeWorldTransform(topLevel);
}
}  // namespace soil::stage::scene::gui
