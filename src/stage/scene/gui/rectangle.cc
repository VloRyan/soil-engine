#include "stage/scene/gui/rectangle.h"

#include "stage/scene/component/transform_component.h"
#include "stage/scene/gui/layout/sizer.h"
#include "stage/scene/gui/root.h"

namespace soil::stage::scene::gui {
Rectangle::Rectangle(Rectangle::SizeTypes sizeType)
    : Rectangle(glm::ivec2(0), sizeType) {}

Rectangle::Rectangle(const glm::ivec2& size, SizeTypes sizeType)
    : Node(Type::Game),
      isMouseOver_(false),
      size_(size),
      minSize_(glm::ivec2(0)),
      maxSize_(glm::ivec2(std::numeric_limits<int>::max())),
      relativeSize_(glm::vec2(0.F)),
      aspectRatio_(0.F),
      padding_(0.F),
      visible_(true),
      visibleEffective_(true),
      onMouseOverFunc_(nullptr),
      onMouseOutFunc_(nullptr),
      sizeType_(sizeType),
      childrenSize_(0) {}

Rectangle* Rectangle::GetParentRect() const {
  return dynamic_cast<Rectangle*>(GetParent());
}

void Rectangle::SetSize(const glm::ivec2& size) {
  auto newSize = glm::clamp(size, minSize_, maxSize_);
  if (size_ == newSize) {
    return;
  }
  size_ = newSize;
  SetDirty(DirtyImpact::Dependents);
}

const glm::ivec2& Rectangle::GetSize() const { return size_; }

const glm::vec2& Rectangle::GetRelativeSize() const { return relativeSize_; }

void Rectangle::SetRelativeSize(const glm::vec2& relativeSize) {
  relativeSize_ = relativeSize;
  if (relativeSize_ != glm::vec2(0.F)) {
    SetSizeType(SizeTypes::Relative);
  }
}

void Rectangle::OnMouseOver(const glm::ivec2& pos) {
  if (!IsVisible()) {
    return;
  }
  isMouseOver_ = true;
  if (onMouseOverFunc_ != nullptr) {
    onMouseOverFunc_(pos);
  }
  for (auto* child : childRects_) {
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
  for (auto* child : childRects_) {
    child->OnMouseOut();
  }
  if (onMouseOutFunc_ != nullptr) {
    onMouseOutFunc_();
  }
}

void Rectangle::OnMouseButton(const glm::ivec2& pos,
                              const input::MouseButton button,
                              input::Event::StateType state) {
  if (!IsVisible()) {
    return;
  }
  for (auto* child : childRects_) {
    if (child->Contains(pos)) {
      child->OnMouseButton(pos, button, state);
    }
  }
}

void Rectangle::OnMouseWheel(const glm::ivec2& pos, const glm::vec2 offset) {
  if (!IsVisible()) {
    return;
  }
  for (auto* child : childRects_) {
    if (child->Contains(pos)) {
      child->OnMouseWheel(pos, offset);
    }
  }
}

void Rectangle::SetAnchor(const layout::Alignment& alignment) {
  anchor_.SetAlignment(alignment);
  MarkDirtyWith(DirtyImpact::Dependents);
}

float Rectangle::GetAspectRatio() const { return aspectRatio_; }

void Rectangle::SetAspectRatio(float aspectRatio) {
  // TODO: gives strange results if maxSize is not in same ratio
  aspectRatio_ = aspectRatio;
}

bool Rectangle::IsMouseOver() const { return isMouseOver_; }

void Rectangle::addChildRect(Rectangle* rect) {
  childRects_.push_back(rect);
  addedRects_.push_back(rect);
  if (auto* parent = GetParentRect(); parent != nullptr) {
    while (parent->GetParentRect() != nullptr) {
      parent = parent->GetParentRect();
    }
    parent->SetDirty(Node::DirtyImpact::Dependents);
  } else {
    SetDirty(Node::DirtyImpact::Dependents);
  }
}

void Rectangle::OnChildAdded(Node* node) {
  if (auto* rect = dynamic_cast<Rectangle*>(node); rect != nullptr) {
    addChildRect(rect);
  }
}

void Rectangle::OnChildRemoved(Node* node) {
  /*auto* rect = dynamic_cast<Rectangle*>(node);
  if (rect != nullptr) {
    removeChildRect(rect);
  }*/
  for (auto itr = childRects_.begin(); itr != childRects_.end(); ++itr) {
    if (*itr == node) {
      childRects_.erase(itr);
      break;
    }
  }
  for (auto itr = addedRects_.begin(); itr != addedRects_.end(); ++itr) {
    if (*itr == node) {
      addedRects_.erase(itr);
      break;
    }
  }
}

void Rectangle::UpdateDirty() {
  if (const auto* parent = GetParentRect(); parent != nullptr) {
    UpdateVisibility(parent->IsVisible());
    if (visibleEffective_) {
      if (IsDirtyImpact(DirtyImpact::Transform)) {
        transform_->UpdateTransform(parent->Transform().GetMatrix());
      }
      UpdateSize(parent->CalculateMaxChildrenSize());
      ApplyAnchors();
      UpdateScissor(parent->CalculateChildScissorRect());
    }
  } else {
    //UpdateChildrenSize(CalculateSize(GetSize()));
    childrenSize_ = CalculateAlignedChildrenSize(GetSize() - Paddings());
  }
  if (visibleEffective_) {
    Layout();
    addedRects_.clear();
  }
  BeforeNodeUpdate();
  Node::UpdateDirty();
  AfterNodeUpdate();
}

void Rectangle::UpdateScissor(const video::render::Rect& parentRect) {
  auto size = glm::ivec2(GetSize());
  const auto halfSize = size / 2;
  const auto maxParentPos = parentRect.LowerLeftPosition + parentRect.Size;
  const auto bottomLeftPos = glm::ivec2(GetPosition()) - halfSize;

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
  SetDirty(DirtyImpact::Dependents);
}

glm::ivec2 Rectangle::CalculateSize(const glm::ivec2& maxSize) {
  switch (sizeType_) {
    case Rectangle::SizeTypes::Fixed:
      return glm::clamp(size_, minSize_, maxSize_);
    case Rectangle::SizeTypes::Relative: {
      /*auto ownMaxSize = glm::max(minSize_, maxSize);//glm::min(maxSize_, maxSize);
      for (auto i = 0; i < 2; i++) {
        if (relativeSize_[i] > 0.F) {
          ownMaxSize[i] = static_cast<int>(std::min(static_cast<float>(maxSize[i]) * relativeSize_[i],
                                                    static_cast<float>(maxSize_[i])));
        }
        if (aspectRatio_ != 0.F) {
          if (relativeSize_.x > 0.F && relativeSize_.y <= 0.F) {
            ownMaxSize.y = static_cast<int>(static_cast<float>(ownMaxSize.x) / aspectRatio_);
          }
          if (relativeSize_.y > 0.F && relativeSize_.x <= 0.F) {
            ownMaxSize.x = static_cast<int>(static_cast<float>(ownMaxSize.y) * aspectRatio_);
          }
        }
      }*/
      auto ownMaxSize = glm::min(maxSize_, maxSize);
      auto relativeMaxSize = layout::Sizer::CalculateRelativeSize(
          GetRelativeSize(), GetAspectRatio(), ownMaxSize, ownMaxSize);
      auto childrenSize = CalculateChildrenSize(relativeMaxSize);
      return glm::clamp(layout::Sizer::CalculateRelativeSize(
                            GetRelativeSize(), GetAspectRatio(), maxSize,
                            glm::max(childrenSize + Paddings(), minSize_)),
                        minSize_, maxSize_);
    }
    case Rectangle::SizeTypes::GrowWithContent: {
      auto childrenSize = CalculateChildrenSize(maxSize);
      return glm::clamp(childrenSize + Paddings(), minSize_, maxSize_);
    }
  }
}

void Rectangle::UpdateSize(const glm::ivec2& maxSize) {
  //auto prevSize = GetSize();
  //UpdateChildrenSize(maxSize);
  // Update children size
  auto& childrenToUpdate = childRects_;
  updateChildrenSize(maxSize, childrenToUpdate);
  SetSize(CalculateSize(maxSize));
  // TODO: Always update all children?


}

void Rectangle::updateChildrenSize(const glm::ivec2& maxSize, std::vector<Rectangle*>& children) {
  auto maxChildSize = glm::ivec2(0);
  switch (sizeType_) {
    case Rectangle::SizeTypes::Fixed:
      maxChildSize = size_;
      break;
    case Rectangle::SizeTypes::Relative: {
      maxChildSize = glm::min(maxSize_, layout::Sizer::CalculateRelativeSize(
          GetRelativeSize(), GetAspectRatio(), maxSize, maxSize));
      break;
    }
    case Rectangle::SizeTypes::GrowWithContent: {
      maxChildSize = maxSize;
      break;
    }
  }
  maxChildSize = glm::min(maxChildSize, maxSize_) - Paddings();
  for (auto* child : children) {
    if (!child->IsVisible()) {
      continue;
    }
    child->UpdateSize(maxChildSize);
  }
  childrenSize_ = CalculateAlignedChildrenSize(maxChildSize);
}

void Rectangle::UpdateChildrenSize(const glm::ivec2& maxSize) {
  switch (sizeType_) {
    case Rectangle::SizeTypes::Fixed:
      childrenSize_ = CalculateChildrenSize(size_);
      break;
    case Rectangle::SizeTypes::Relative: {
      auto ownMaxSize = glm::min(maxSize_, maxSize);
      auto relativeMaxSize = layout::Sizer::CalculateRelativeSize(
          GetRelativeSize(), GetAspectRatio(), ownMaxSize, ownMaxSize);
      childrenSize_ = CalculateChildrenSize(relativeMaxSize);
      break;
    }
    case Rectangle::SizeTypes::GrowWithContent: {
      childrenSize_ = CalculateChildrenSize(maxSize);
      break;
    }
  }
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
  if (auto* parent = GetParentRect(); parent != nullptr) {
    UpdateVisibility(parent->IsVisible());
  } else {
    UpdateVisibility(visible);
  }
}

bool Rectangle::IsVisible() const { return visibleEffective_; }

void Rectangle::SetOnMouseOverFunc(
    const std::function<void(const glm::ivec2& pos)>& onMouseOverFunc) {
  onMouseOverFunc_ = onMouseOverFunc;
}

void Rectangle::SetOnMouseOutFunc(const std::function<void()>& onMouseOutFunc) {
  onMouseOutFunc_ = onMouseOutFunc;
}

const video::render::Rect& Rectangle::GetScissorRect() const {
  return scissorRect_;
}

void Rectangle::ApplyAnchors() {
  if (GetParentRect() == nullptr) {
    return;
  }
  SetLocalPosition(anchor_.Align(GetLocalPosition(), GetSize(),
                                 GetParentRect()->GetSize(),
                                 GetParentRect()->GetPadding()));
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

void Rectangle::SetMinSize(const glm::ivec2& minSize) {
  auto size = glm::max(glm::ivec2(0), minSize);
  if (minSize_ == size) {
    return;
  }
  minSize_ = size;
  SetDirty(DirtyImpact::Dependents);
}

glm::ivec2 Rectangle::GetMaxSize() const { return maxSize_; }

void Rectangle::SetMaxSize(const glm::ivec2& maxSize) {
  auto size = maxSize;
  for (auto i = 0; i < 2; i++) {
    if (size[i] < 0) {
      size[i] = std::numeric_limits<int>::max();
    }
  }
  if (maxSize_ == size) {
    return;
  }
  maxSize_ = size;
  SetDirty(DirtyImpact::Dependents);
}

class Root* Rectangle::GuiRoot() const {
  auto p = const_cast<Rectangle*>(this);
  while (p->GetParentRect() != nullptr) {
    p = p->GetParentRect();
  }
  return dynamic_cast<class Root*>(p);
}

void Rectangle::FindChildrenAt(std::vector<Rectangle*>& result, glm::ivec2 pos,
                               bool onlyVisible) {
  for (auto* child : childRects_) {
    if (onlyVisible && !child->IsVisible() || !child->Contains(pos)) {
      continue;
    }
    result.push_back(child);
  }
}

void Rectangle::Layout() {
  for (auto* child : childRects_) {
    if (!child->IsVisible()) {
      continue;
    }
    auto localPos = child->GetLocalPosition();
    child->SetLocalPosition(glm::vec3(localPos.x, localPos.y, LAYER_Z_INCREMENT));
  }
}

Rectangle::SizeTypes Rectangle::GetSizeType() const { return sizeType_; }

void Rectangle::SetSizeType(Rectangle::SizeTypes sizeType) {
  sizeType_ = sizeType;
}

glm::ivec2 Rectangle::CalculateAlignedChildrenSize(const glm::ivec2& maxSize) const {
  auto size = minSize_;
  if (childRects_.empty()) {
    return size;
  }
  for (auto* child : childRects_) {
    if (!child->IsVisible()) {
      continue;
    }
    size = glm::max(child->GetSize(), size);
  }
  return size;
}

glm::ivec2 Rectangle::CalculateChildrenSize(const glm::ivec2& maxSize) {
  auto size = minSize_;
  if (childRects_.empty()) {
    return size;
  }
  auto maxChildSize = glm::min(maxSize, maxSize_) - Paddings();
  for (auto* child : childRects_) {
    if (!child->IsVisible()) {
      continue;
    }
    auto childSize = child->CalculateSize(maxChildSize);
    size = glm::max(childSize, size);
  }
  return size;
}

const glm::ivec2& Rectangle::GetChildrenSize() const { return childrenSize_; }

video::render::Rect Rectangle::CalculateChildScissorRect() const {
  return video::render::Rect{
      .LowerLeftPosition = scissorRect_.LowerLeftPosition + glm::ivec2(padding_[0], padding_[3]),
      .Size = scissorRect_.Size - Paddings(),
  };
}

glm::ivec2 Rectangle::Paddings() const {
  return {padding_[0] + padding_[2], padding_[1] + padding_[3]};
}

void Rectangle::UpdateVisibility(bool parentVisible) {
  const auto visibleEffective = visible_ && parentVisible;
  if (visibleEffective_ == visibleEffective) {
    return;
  }
  visibleEffective_ = visibleEffective;
  for (auto* child : childRects_) {
    child->UpdateVisibility(visibleEffective_);
  }
  if (auto* parent = GetParentRect(); parent != nullptr) {
    while (parent->GetParentRect() != nullptr) {
      parent = parent->GetParentRect();
    }
    parent->SetDirty(Node::DirtyImpact::Dependents);
  } else {
    SetDirty(Node::DirtyImpact::Dependents);
  }
}

glm::ivec2 Rectangle::CalculateMaxChildrenSize() const {
  switch (sizeType_) {
    case SizeTypes::Fixed:
      return glm::min(GetSize() - Paddings(), maxSize_);
    case SizeTypes::Relative: {
      glm::ivec2 parentSize;
      if (auto* parent = GetParentRect();parent != nullptr) {
        parentSize = GetParentRect()->CalculateMaxChildrenSize();
      } else {
        parentSize = maxSize_ - Paddings();
      }
      auto childSize = glm::min(GetSize(), maxSize_ - Paddings());
      if (aspectRatio_ == 0.F) {
        if (relativeSize_.x == 0.F) {
          childSize.x = std::min(parentSize.x, maxSize_.x - Paddings().x);
        }
        if (relativeSize_.y == 0.F) {
          childSize.y = std::min(parentSize.y, maxSize_.y - Paddings().y);
        }
      }
      return childSize;
    }
    case SizeTypes::GrowWithContent:
      if (auto* parent = GetParentRect();parent != nullptr) {
        return glm::min(GetParentRect()->CalculateMaxChildrenSize(), maxSize_) - Paddings();
      }
      return maxSize_ - Paddings();
  }

}

}  // namespace soil::stage::scene::gui
