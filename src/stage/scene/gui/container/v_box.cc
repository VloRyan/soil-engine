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
  if (children_.empty()) {
    return;
  }

  auto offset = glm::ivec2(GetOffset().x, -padding_[1] + GetOffset().y);
  const auto halfHeight = GetSize().y / 2;
  for (auto* item : children_) {
    auto pos = item->GetLocalPosition();
    pos.z = 0.F;
    pos.x = static_cast<float>(offset.x);
    pos.y = static_cast<float>(offset.y) + static_cast<float>(halfHeight) -
            static_cast<float>(item->GetSize().y) / 2.F;
    switch (alignItems_) {
      case AlignItems::Left:
        pos.x -= static_cast<float>(GetSize().x) / 2.F -
                 static_cast<float>(padding_[0]) -
                 static_cast<float>(item->GetSize().x) / 2.F;
        break;
      case AlignItems::Center:
        pos.x +=
            static_cast<float>(padding_[0]) - static_cast<float>(padding_[2]);
        break;
      case AlignItems::Right:
        pos.x += static_cast<float>(GetSize().x) / 2.F -
                 static_cast<float>(padding_[2]) -
                 static_cast<float>(item->GetSize().x) / 2.F;
        break;
    }
    item->SetLocalPosition(pos);
    offset.y -= item->GetSize().y + margin_;
    itemsSize_.y += item->GetSize().y;
    if (item->GetSize().x > itemsSize_.x) {
      itemsSize_.x = item->GetSize().x;
    }
  }
  itemsSize_.y += (static_cast<int>(children_.size()) - 1) * margin_;
}
}  // namespace soil::stage::scene::gui::container
