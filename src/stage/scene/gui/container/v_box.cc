#include "stage/scene/gui/container/v_box.h"

namespace soil::stage::scene::gui::container {

VBox::VBox(const int margin, const glm::ivec4 padding)
    : Base(margin, padding), alignItems_(AlignItems::Center) {}

VBox::AlignItems VBox::GetAlignItems() const { return alignItems_; }

void VBox::SetAlignItems(const AlignItems alignItems) {
  if (alignItems_ == alignItems) {
    return;
  }
  alignItems_ = alignItems;
  SetDirty(DirtyImpact::Dependents);
}

void VBox::arrangeItems() {
  itemsSize_ = glm::vec2(0.F);
  if (items_.empty()) {
    return;
  }

  auto offset = glm::ivec2(GetOffset().x, -padding_[1] + GetOffset().y);
  const auto halfHeight = GetSize().y / 2;
  for (auto* item : items_) {
    auto pos = item->GetPosition();
    pos.x = offset.x;
    pos.y = offset.y + halfHeight - item->GetSize().y / 2;
    switch (alignItems_) {
      case AlignItems::Left:
        pos.x -= GetSize().x / 2 - padding_[0] - item->GetSize().x / 2;
        break;
      case AlignItems::Center:
        pos.x += padding_[0] - padding_[2];
        break;
      case AlignItems::Right:
        pos.x += GetSize().x / 2 - padding_[2] - item->GetSize().x / 2;
        break;
    }
    item->SetPosition(pos);
    offset.y -= item->GetSize().y + margin_;
    itemsSize_.y += item->GetSize().y;
    if (item->GetSize().x > itemsSize_.x) {
      itemsSize_.x = item->GetSize().x;
    }
  }
  itemsSize_.y += (static_cast<float>(items_.size()) - 1.F) * margin_;
}
}  // namespace soil::stage::scene::gui::container
