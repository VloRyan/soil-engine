#include "stage/scene/gui/container/base.h"

namespace soil::stage::scene::gui::container {

Base::Base(const int margin, const glm::ivec4 padding, SizeTypes sizeType)
    : Rectangle(glm::ivec2(0), sizeType),
      margin_(margin),
      offset_(glm::ivec2(0)), scrollStep_(glm::ivec2(10)), itemAlignment_({layout::Alignment::Horizontal::Center,
                                                                           layout::Alignment::Vertical::Center}) {
  Rectangle::SetPadding(padding);
}

const glm::ivec2& Base::GetScrollStep() const {
  return scrollStep_;
}

void Base::SetScrollStep(const glm::ivec2& scrollStep) {
  scrollStep_ = scrollStep;
}

const std::vector<Rectangle*>& Base::GetItems() const { return childRects_; }

const glm::ivec2& Base::GetOffset() const { return offset_; }

void Base::SetOffset(const glm::ivec2& offset) {
  if (offset == offset_) {
    return;
  }
  offset_ = offset;
  SetDirty(DirtyImpact::Dependents);
}

int Base::GetMargin() const { return margin_; }

void Base::SetMargin(const int margin) {
  if (margin == margin_) {
    return;
  }
  margin_ = margin;
  SetDirty(DirtyImpact::Dependents);
}

void Base::addChildRect(Rectangle* rect) {
  Rectangle::addChildRect(rect);
  SetDirty(Node::DirtyImpact::Dependents);
}

void Base::RemoveChild(Node* node) {
  Rectangle::RemoveChild(node);
  SetDirty(Node::DirtyImpact::Dependents);
}

Rectangle* Base::Child(int index) { return childRects_.at(index); }

void Base::OnMouseWheel(const glm::ivec2& pos, glm::vec2 offset) {
  if (scrollStep_ == glm::ivec2(0)) {
    return;
  }
  auto direction = glm::ivec2(0);
  for (auto i = 0; i < 2; i++) {
    if (offset[i] > 0.F) {
      direction[i] = 1;
    }
    if (offset[i] < 0.F) {
      direction[i] = -1;
    }
  }
  direction.y *= -1;
  auto newValue = GetOffset();
  newValue += scrollStep_ * direction;
  const auto overflow = GetSize() - GetScissorRect().Size;
  newValue = glm::clamp(newValue, glm::ivec2(0), overflow);
  SetOffset(newValue);
}

const layout::Alignment& Base::GetItemAlignment() const {
  return itemAlignment_;
}

void Base::SetItemAlignment(
    const layout::Alignment alignment) {
  if (itemAlignment_ == alignment) {
    return;
  }
  itemAlignment_ = alignment;
  SetDirty(DirtyImpact::Dependents);
}

}  // namespace soil::stage::scene::gui::container
