#include "stage/scene/gui/container/h_box.h"

namespace soil::stage::scene::gui::container {

HBox::HBox(const int margin, const glm::ivec4 padding) : Base(margin, padding) {
  itemAnchor_.SetAlignment(
      {.Vertical = layout::Anchor::VerticalAlignments::Center});
}

layout::Anchor::VerticalAlignments HBox::GetItemAlignment() const {
  return itemAnchor_.GetAlignment().Vertical;
}

void HBox::SetItemAlignment(
    const layout::Anchor::VerticalAlignments alignment) {
  if (itemAnchor_.GetAlignment().Vertical == alignment) {
    return;
  }
  itemAnchor_.SetAlignment({.Vertical = alignment});
  SetDirty(DirtyImpact::Dependents);
}

void HBox::Layout() {
  if (childRects_.empty()) {
    return;
  }
  const auto halfSize = glm::vec2(GetSize()) / glm::vec2(2.F);
  auto offset =
      glm::vec3(static_cast<float>(padding_[0] + GetOffset().x) - halfSize.x,
                GetOffset().y, Rectangle::LAYER_Z_INCREMENT);
  for (auto* child : childRects_) {
    auto halfItemSize = glm::vec2(child->GetSize()) / glm::vec2(2.F);
    auto pos = offset + glm::vec3(halfItemSize.x, 0.F, 0.F);
    child->SetLocalPosition(
        itemAnchor_.Align(pos, child->GetSize(), GetSize(), GetPadding()));
    offset.x += static_cast<float>(child->GetSize().x + margin_);
  }
}

glm::ivec2 HBox::CalculateChildrenSize(const glm::ivec2& maxSize) {
  glm::ivec2 size(0);
  if (childRects_.empty()) {
    return size;
  }
  auto maxChildSize = glm::clamp(maxSize, minSize_, maxSize_) - Paddings();
  for (auto* child : childRects_) {
    auto childSize = child->CalculateSize(maxChildSize);
    size.x += childSize.x;
    size.y = std::max(childSize.y, size.y);
  }
  size.x += (static_cast<int>(childRects_.size()) - 1) * margin_;
  return size;
}
}  // namespace soil::stage::scene::gui::container
