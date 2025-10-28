#include "stage/scene/gui/container/h_box.h"

namespace soil::stage::scene::gui::container {

HBox::HBox(const int margin, const glm::ivec4 padding)
    : Base(margin, padding), alignItems_(AlignItems::Center) {}

HBox::AlignItems HBox::GetAlignItems() const { return alignItems_; }

void HBox::SetAlignItems(const AlignItems alignItems) {
  if (alignItems_ == alignItems) {
    return;
  }
  alignItems_ = alignItems;
  SetDirty(DirtyImpact::Dependents);
}

void HBox::arrangeItems() {
  itemsSize_ = glm::vec2(0.F);
  if (children_.empty()) {
    return;
  }

  auto offset = glm::ivec2(padding_[0] + GetOffset().x, GetOffset().y);
  const auto halfWidth = GetSize().x / 2;
  for (auto* item : children_) {
    auto pos = item->GetLocalPosition();
    pos.z = 0.F;
    pos.x = static_cast<float>(offset.x) - static_cast<float>(halfWidth) +
            static_cast<float>(item->GetSize().x) / 2.F;
    pos.y = static_cast<float>(offset.y);
    switch (alignItems_) {
      case AlignItems::Top:
        pos.y += static_cast<float>(GetSize().y) / 2.F -
                 static_cast<float>(padding_[1]) -
                 static_cast<float>(item->GetSize().y) / 2.F;
        break;
      case AlignItems::Center:
        pos.y +=
            static_cast<float>(padding_[1]) - static_cast<float>(padding_[3]);
        break;
      case AlignItems::Bottom:
        pos.y -= static_cast<float>(GetSize().y) / 2.F -
                 static_cast<float>(padding_[3]) -
                 static_cast<float>(item->GetSize().y) / 2.F;
        break;
    }
    item->SetLocalPosition(pos);
    offset.x += item->GetSize().x + margin_;
    itemsSize_.x += item->GetSize().x;
    if (item->GetSize().y > itemsSize_.y) {
      itemsSize_.y = item->GetSize().y;
    }
  }
  itemsSize_.x += (static_cast<int>(children_.size()) - 1) * margin_;
}
}  // namespace soil::stage::scene::gui::container
