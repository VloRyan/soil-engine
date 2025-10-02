#include "stage/scene/gui/container/base.h"

namespace soil::stage::scene::gui::container {

Base::Base(const int margin, const glm::ivec4 padding)
    : margin_(margin), offset_(glm::ivec2(0)), itemsSize_(glm::ivec2(0)) {
  Rectangle::SetPadding(padding);
}

Base::~Base() {
  for (auto* item : items_) {
    item->RemoveListener(this);
  }
  items_.clear();
}

const std::vector<Rectangle*>& Base::GetItems() const { return items_; }

void Base::Handle(const event::Node& event) {
  if (event.GetChangeType() == event::Node::ChangeType::State &&
      event.GetOrigin()->GetState() == State::Dirty) {
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

void Base::addChild(Node* node) { addChild(node, true); }

void Base::addChild(Node* node, const bool asItem) {
  if (node->GetParent() == this) {
    return;
  }
  Rectangle::addChild(node);
  if (!asItem) {
    return;
  }
  if (auto* item = dynamic_cast<Rectangle*>(node); item != nullptr) {
    items_.push_back(item);
    item->AddListener(this);
  }
}

void Base::RemoveChild(Node* node) {
  for (auto it = items_.begin(); it != items_.end(); ++it) {
    if (*it == node) {
      (*it)->RemoveListener(this);
      items_.erase(it);
      break;
    }
  }
  Rectangle::RemoveChild(node);
}

void Base::BeforeNodeUpdate() {
  for (auto* item : items_) {
    item->UpdateSize(GetChildSize());
  }
  arrangeItems();
}

}  // namespace soil::stage::scene::gui::container
