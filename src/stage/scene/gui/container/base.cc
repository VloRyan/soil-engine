#include "stage/scene/gui/container/base.h"

namespace soil::stage::scene::gui::container {

Base::Base(const int margin, const glm::ivec4 padding)
    : margin_(margin), offset_(glm::ivec2(0)), itemsSize_(glm::ivec2(0)) {
  Rectangle::SetPadding(padding);
}

const std::vector<Rectangle*>& Base::GetItems() const { return childRects_; }

void Base::Handle(const event::Node& event) {
  if (event.ChangeType == event::Node::ChangeType::State &&
      event.Origin->GetState() == State::Dirty) {
    SetDirty(DirtyImpact::Dependents);
  }
}

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

const glm::ivec2& Base::GetItemsSize() const { return itemsSize_; }

void Base::addChild(Node* node) {
  if (node->GetParent() == this) {
    return;
  }
  Rectangle::addChild(node);
  SetDirty(Node::DirtyImpact::Dependents);
}

void Base::RemoveChild(Node* node) {
  Rectangle::RemoveChild(node);
  SetDirty(Node::DirtyImpact::Dependents);
}

void Base::AfterNodeUpdate() { arrangeItems(); }

}  // namespace soil::stage::scene::gui::container
