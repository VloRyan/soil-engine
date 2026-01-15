#include "stage/scene/gui/container/base.h"

namespace soil::stage::scene::gui::container {

Base::Base(const int margin, const glm::ivec4 padding, SizeTypes sizeType)
    : Rectangle(glm::ivec2(0), sizeType),
      margin_(margin),
      offset_(glm::ivec2(0)) {
  Rectangle::SetPadding(padding);
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

}  // namespace soil::stage::scene::gui::container
