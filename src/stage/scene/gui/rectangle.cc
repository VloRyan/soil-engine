#include "stage/scene/gui/rectangle.h"

#include "stage/scene/component/transform_component.h"
#include "stage/scene/gui/root.h"

namespace soil::stage::scene::gui {
Rectangle::Rectangle()
    : Node(Type::Game),
      isMouseOver_(false),
      size_(glm::ivec2(10)),
      minSize_(glm::ivec2(10)),
      maxSize_(glm::ivec2(0)),
      relativeSize_(glm::vec2(0.F)),
      aspectRatio_(glm::vec2(0.F)),
      padding_(0.F),
      horizontalAnchors_(HorizontalAnchors::None),
      verticalAnchors_(VerticalAnchors::None),
      visible_(true),
      visibleEffective_(true),
      onMouseOverFunc_(nullptr),
      onMouseOutFunc_(nullptr) {
  Node::SetLocalPosition(glm::vec3(0.F, 0.F, LAYER_Z_INCREMENT));
}

Rectangle* Rectangle::GetParentRect() const {
  return dynamic_cast<Rectangle*>(GetParent());
}

void Rectangle::SetSize(const glm::ivec2& size) {
  auto newSize = glm::max(size, minSize_);
  for (auto i = 0; i < 2; i++) {
    if (maxSize_[i] > 0) {
      newSize[i] = std::min(newSize[i], maxSize_[i]);
    }
  }
  if (size_ == newSize) {
    return;
  }
  size_ = newSize;
  SetDirty(DirtyImpact::Dependents);
}

const glm::ivec2& Rectangle::GetSize() const { return size_; }

const glm::vec2& Rectangle::GetRelativeSize() const { return relativeSize_; }

void Rectangle::SetRelativeSize(const glm::vec2& relative_size) {
  relativeSize_ = relative_size;
}

glm::ivec2 Rectangle::GetChildSize() const {
  auto childSize = GetSize();
  childSize.x -= padding_[0] + padding_[2];
  childSize.y -= padding_[1] + padding_[3];
  return childSize;
}

void Rectangle::OnMouseOver(const glm::ivec2& pos) {
  if (!IsVisible()) {
    return;
  }
  isMouseOver_ = true;
  if (onMouseOverFunc_ != nullptr) {
    onMouseOverFunc_(pos);
  }
  for (auto* child : children_) {
    if (child->Contains(pos)) {
      child->OnMouseOver(pos);
    } else {
      if (child->IsMouseOver()) {
        child->OnMouseOut();
      }
    }
  }
}

void Rectangle::OnMouseOut() {
  if (!IsVisible()) {
    return;
  }
  isMouseOver_ = false;
  for (auto* child : children_) {
    child->OnMouseOut();
  }
  if (onMouseOutFunc_ != nullptr) {
    onMouseOutFunc_();
  }
}

void Rectangle::OnMouseButton(const glm::ivec2& pos,
                              const input::MouseButton button,
                              input::Event::StateType state) {
  for (auto* child : children_) {
    if (child->Contains(pos)) {
      child->OnMouseButton(pos, button, state);
    }
  }
}

void Rectangle::OnMouseWheel(const glm::ivec2& pos, const glm::vec2 offset) {
  if (!IsVisible()) {
    return;
  }
  for (auto* child : children_) {
    if (child->Contains(pos)) {
      child->OnMouseWheel(pos, offset);
    }
  }
}

void Rectangle::SetAnchor(const HorizontalAnchors horizontal,
                          const VerticalAnchors vertical) {
  horizontalAnchors_ = horizontal;
  verticalAnchors_ = vertical;
  MarkDirtyWith(DirtyImpact::Dependents);
}

const glm::vec2& Rectangle::GetAspectRatio() const { return aspectRatio_; }

void Rectangle::SetAspectRatio(const glm::vec2& aspectRatio) {
  aspectRatio_ = aspectRatio;
}

bool Rectangle::IsMouseOver() const { return isMouseOver_; }

void Rectangle::addChild(Node* node) {
  if (node->GetParent() == this) {
    return;
  }
  Node::addChild(node);
  auto* rect = dynamic_cast<Rectangle*>(node);
  if (rect != nullptr) {
    addChildRect(rect);
  }
}

void Rectangle::addChildRect(Rectangle* rect) { children_.push_back(rect); }

void Rectangle::removeChildRect(const Rectangle* rect) {
  for (auto itr = children_.begin(); itr != children_.end(); ++itr) {
    if (*itr == rect) {
      children_.erase(itr);
      return;
    }
  }
}

void Rectangle::RemoveChild(Node* node) {
  for (auto itr = children_.begin(); itr != children_.end(); ++itr) {
    if (*itr == node) {
      children_.erase(itr);
      break;
    }
  }
  Node::RemoveChild(node);
}

void Rectangle::UpdateDirty() {
  if (const auto* parent = GetParentRect(); parent != nullptr) {
    if (const auto visibleEffective = visible_ && parent->IsVisible();
        visibleEffective_ != visibleEffective) {
      visibleEffective_ = visibleEffective;
      SetDirty(DirtyImpact::Dependents);
    }
    if (visibleEffective_) {
      UpdateSize(parent->GetChildSize());
      ApplyAnchors();
      if (IsDirtyImpact(DirtyImpact::Transform)) {
        transform_->UpdateTransform(parent->Transform().GetMatrix());
      }
      UpdateScissor(parent->childScissorRect_);
    }
  }
  BeforeNodeUpdate();
  Node::UpdateDirty();
}

void Rectangle::BeforeNodeUpdate() {}

void Rectangle::UpdateScissor(const video::render::Rect& parentRect) {
  auto size = glm::ivec2(GetSize());
  const auto halfSize = GetSize() / 2;
  const auto maxParentPos = parentRect.LowerLeftPosition + parentRect.Size;
  const auto bottomLeftPos = glm::ivec2(glm::ivec2(GetPosition()) - halfSize);

  scissorRect_.LowerLeftPosition = bottomLeftPos;
  scissorRect_.Size = size;
  for (auto i = 0; i < 2; i++) {
    if (scissorRect_.LowerLeftPosition[i] < parentRect.LowerLeftPosition[i]) {
      size[i] = std::max(size[i] - (parentRect.LowerLeftPosition[i] -
                                    scissorRect_.LowerLeftPosition[i]),
                         0);
      scissorRect_.LowerLeftPosition[i] = parentRect.LowerLeftPosition[i];
    }
    if (scissorRect_.LowerLeftPosition[i] > maxParentPos[i]) {
      scissorRect_.LowerLeftPosition[i] = maxParentPos[i];
    }
    auto maxDim = maxParentPos[i] - scissorRect_.LowerLeftPosition[i];
    scissorRect_.Size[i] = std::min(maxDim, size[i]);
  }

  childScissorRect_.LowerLeftPosition =
      scissorRect_.LowerLeftPosition + glm::ivec2(padding_[0], padding_[3]);
  childScissorRect_.Size = GetChildSize();
  for (auto i = 0; i < 2; i++) {
    auto maxDim = maxParentPos[i] - childScissorRect_.LowerLeftPosition[i];
    childScissorRect_.Size[i] = std::min(maxDim, childScissorRect_.Size[i]);
  }
}

void Rectangle::UpdateSize(const glm::ivec2& parentSize) {
  auto newSize = GetSize();
  for (auto i = 0; i < 2; i++) {
    if (relativeSize_[i] > 0.F) {
      newSize[i] = static_cast<int>(static_cast<float>(parentSize[i]) *
                                    relativeSize_[i]);
    }
  }
  if (aspectRatio_.x > 0.F) {
    newSize.x =
        static_cast<int>(static_cast<float>(newSize.y) * aspectRatio_.x);
  } else if (aspectRatio_.y > 0.F) {
    newSize.y =
        static_cast<int>(static_cast<float>(newSize.x) / aspectRatio_.y);
  }
  SetSize(newSize);
}

const glm::ivec4& Rectangle::GetPadding() const { return padding_; }

void Rectangle::SetPadding(const glm::ivec4& padding) {
  if (padding_ == padding) {
    return;
  }
  padding_ = padding;
  SetDirty(DirtyImpact::Dependents);
}

void Rectangle::SetVisible(const bool visible) {
  if (visible == visible_) {
    return;
  }
  if (!visible && IsMouseOver()) {
    OnMouseOut();
  }
  visible_ = visible;
  auto visibleEffective = visible_;
  if (const auto* parent = GetParentRect(); parent != nullptr) {
    visibleEffective = visible_ && parent->IsVisible();
  }
  if (visibleEffective != visibleEffective_) {
    visibleEffective_ = visibleEffective;
    SetDirty(DirtyImpact::Dependents);
  }
}

bool Rectangle::IsVisible() const { return visibleEffective_; }

glm::vec2 Rectangle::GetCenter() const {
  return {padding_[0] - padding_[2], padding_[3] - padding_[1]};
}

void Rectangle::SetOnMouseOverFunc(
    const std::function<void(glm::ivec2 pos)>& onMouseOverFunc) {
  onMouseOverFunc_ = onMouseOverFunc;
}

void Rectangle::SetOnMouseOutFunc(const std::function<void()>& onMouseOutFunc) {
  onMouseOutFunc_ = onMouseOutFunc;
}

const video::render::Rect& Rectangle::GetScissorRect() const {
  return scissorRect_;
}

void Rectangle::ApplyAnchors() {
  glm::vec4 parentPadding = GetParentRect()->GetPadding();
  auto parentSize = GetParentRect()->GetSize();
  auto parentCenter = GetParentRect()->GetCenter();
  if (horizontalAnchors_ == HorizontalAnchors::None &&
      verticalAnchors_ == VerticalAnchors::None) {
    return;
  }
  glm::vec3 pos{parentCenter.x, parentCenter.y, GetLocalPosition().z};
  const glm::vec2 parentHalfSize = parentSize / glm::ivec2(2);
  const glm::vec2 halfSize = GetSize() / glm::ivec2(2);
  switch (horizontalAnchors_) {
    case HorizontalAnchors::Left:
      pos.x = -parentHalfSize.x + halfSize.x + parentPadding[0];
      break;
    case HorizontalAnchors::Right:
      pos.x = parentHalfSize.x - halfSize.x - parentPadding[2];
      break;
    case HorizontalAnchors::Center:
    case HorizontalAnchors::None:
      pos.x = (parentPadding[0] - parentPadding[2]);
      break;
  }
  switch (verticalAnchors_) {
    case VerticalAnchors::Top:
      pos.y = parentHalfSize.y - halfSize.y - parentPadding[1];
      break;
    case VerticalAnchors::Bottom:
      pos.y = -parentHalfSize.y + halfSize.y + parentPadding[3];
      break;
    case VerticalAnchors::Middle:
    case VerticalAnchors::None:
      pos.y = (-parentPadding[1] + parentPadding[3]);
  }
  SetLocalPosition(pos);
}

bool Rectangle::Contains(const glm::ivec2 pos) const {
  const auto halfSize = glm::ivec2(GetSize()) / glm::ivec2(2);
  const auto cPos = glm::ivec2(GetPosition());
  return pos.x >= cPos.x - halfSize.x &&  //
         pos.x <= cPos.x + halfSize.x &&  //
         pos.y >= cPos.y - halfSize.y &&  //
         pos.y <= cPos.y + halfSize.y;
}

bool Rectangle::Contains(const Rectangle* other) const {
  const auto halfSize = glm::ivec2(GetSize()) / glm::ivec2(2);
  const auto otherHalfSize = glm::ivec2(other->GetSize()) / glm::ivec2(2);
  const auto pos = glm::ivec2(GetPosition());
  const auto otherPos = glm::ivec2(GetPosition());
  const bool betweenX = (otherPos.x + otherHalfSize.x >= pos.x - halfSize.x &&
                         otherPos.x + otherHalfSize.x <= pos.x + halfSize.x) ||
                        (otherPos.x - otherHalfSize.x <= pos.x + halfSize.x &&
                         otherPos.x - otherHalfSize.x >= pos.x - halfSize.x);
  const bool betweenY = (otherPos.y + otherHalfSize.y >= pos.y - halfSize.y &&
                         otherPos.y + otherHalfSize.y <= pos.y + halfSize.y) ||
                        (otherPos.y - otherHalfSize.y <= pos.y + halfSize.y &&
                         otherPos.y - otherHalfSize.y >= pos.y - halfSize.y);
  return betweenX && betweenY;
}

glm::ivec2 Rectangle::GetMinSize() const { return minSize_; }

void Rectangle::SetMinSize(const glm::ivec2& minSize) { minSize_ = minSize; }

glm::ivec2 Rectangle::GetMaxSize() const { return maxSize_; }

void Rectangle::SetMaxSize(const glm::ivec2& maxSize) { maxSize_ = maxSize; }

class Root* Rectangle::GuiRoot() const {
  auto p = const_cast<Rectangle*>(this);
  while (p->GetParentRect() != nullptr) {
    p = p->GetParentRect();
  }
  return dynamic_cast<class Root*>(p);
}
void Rectangle::FindChildrenAt(std::vector<Rectangle*>& result, glm::ivec2 pos,
                               bool onlyVisible) {
  for (auto* child : children_) {
    if (onlyVisible && !child->IsVisible() || !child->Contains(pos)) {
      continue;
    }
    result.push_back(child);
  }
}

}  // namespace soil::stage::scene::gui
