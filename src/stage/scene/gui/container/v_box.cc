#include "stage/scene/gui/container/v_box.h"

#include "stage/scene/gui/rectangle.h"
namespace soil::stage::scene::gui::container {

VBox::VBox(const int margin, const glm::ivec4 padding) : Base(margin, padding) {
  itemAnchor_.SetAlignment(
      {.Horizontal = layout::Anchor::HorizontalAlignments::Center});
}

layout::Anchor::HorizontalAlignments VBox::GetItemAlignment() const {
  return itemAnchor_.GetAlignment().Horizontal;
}

void VBox::SetItemAlignment(
    const layout::Anchor::HorizontalAlignments alignment) {
  if (itemAnchor_.GetAlignment().Horizontal == alignment) {
    return;
  }
  itemAnchor_.SetAlignment({.Horizontal = alignment});
  SetDirty(DirtyImpact::Dependents);
}

void VBox::Layout() {
  if (childRects_.empty()) {
    return;
  }
  const auto halfSize = glm::vec2(GetSize()) / glm::vec2(2.F);
  auto offset =
      glm::vec3(-GetOffset().x,
                halfSize.y - static_cast<float>(-GetOffset().y + padding_[1]),
                Rectangle::LAYER_Z_INCREMENT);
  for (auto* child : childRects_) {
    auto halfItemSize = glm::vec2(child->GetSize()) / glm::vec2(2.F);
    auto pos = offset + glm::vec3(0.F, -halfItemSize.y, 0.F);
    child->SetLocalPosition(
        itemAnchor_.Align(pos, child->GetSize(), GetSize(), GetPadding()));
    offset.y -= static_cast<float>(child->GetSize().y + margin_);
  }
}

glm::ivec2 VBox::CalculateChildrenSize(const glm::ivec2& maxSize) {
  glm::ivec2 size(minSize_.x, 0);
  if (childRects_.empty()) {
    return size;
  }
  auto maxChildSize = glm::min(maxSize, maxSize_) - Paddings();
  for (auto* child : childRects_) {
    if (!child->IsVisible()) {
      continue;
    }
    auto childSize = child->CalculateSize(maxChildSize);
    size.x = std::max(childSize.x, size.x);
    size.y += childSize.y;
  }
  size.y += (static_cast<int>(childRects_.size()) - 1) * margin_;
  return size;
}
}  // namespace soil::stage::scene::gui::container
