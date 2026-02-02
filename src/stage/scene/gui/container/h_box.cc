#include "stage/scene/gui/container/h_box.h"

namespace soil::stage::scene::gui::container {

HBox::HBox(const int margin, const glm::ivec4 padding) : Base(margin, padding) {
}

void HBox::Layout() {
  if (childRects_.empty()) {
    return;
  }
  const auto halfSize = glm::vec2(GetSize()) / glm::vec2(2.F);
  auto offset = glm::vec3(GetOffset(), Rectangle::LAYER_Z_INCREMENT);
  switch (itemAlignment_.X) {
    case layout::Alignment::Horizontal::Right:
      offset.x += halfSize.x - static_cast<float>(padding_[2]) - static_cast<float>(childrenSize_.x);
      break;
    case layout::Alignment::Horizontal::Left:
      offset.x -= halfSize.x - static_cast<float>(padding_[0]);
      break;
    case layout::Alignment::Horizontal::Center:
    case layout::Alignment::Horizontal::None:
      offset.x -= static_cast<float>(childrenSize_.x) * 0.5F;
      break;
  }
  for (auto* child : childRects_) {
    if (!child->IsVisible()) {
      continue;
    }
    auto halfItemSize = glm::vec2(child->GetSize()) / glm::vec2(2.F);
    auto pos = offset + glm::vec3(halfItemSize.x, 0.F, 0.F);
    child->SetLocalPosition(
        layout::Anchor::Align({.Y=itemAlignment_.Y}, pos, child->GetSize(), GetSize(), GetPadding()));
    offset.x += static_cast<float>(child->GetSize().x + margin_);
  }
}

glm::ivec2 HBox::CalculateChildrenSize(const glm::ivec2& maxSize) {
  glm::ivec2 size(0, minSize_.y);
  if (childRects_.empty()) {
    return size;
  }
  auto maxChildSize = glm::min(maxSize, maxSize_) - Paddings();
  for (auto* child : childRects_) {
    if (!child->IsVisible()) {
      continue;
    }
    auto childSize = child->CalculateSize(maxChildSize);
    size.x += childSize.x;
    size.y = std::max(childSize.y, size.y);
  }
  size.x += (static_cast<int>(childRects_.size()) - 1) * margin_;
  return size;
}

glm::ivec2 HBox::CalculateAlignedChildrenSize(const glm::ivec2& maxSize) const {
  glm::ivec2 size(0, minSize_.y);
  if (childRects_.empty()) {
    return size;
  }
  for (auto* child : childRects_) {
    if (!child->IsVisible()) {
      continue;
    }
    auto childSize = child->GetSize();
    size.x += childSize.x;
    size.y = std::max(childSize.y, size.y);
  }
  size.x += (static_cast<int>(childRects_.size()) - 1) * margin_;
  return size;
}
}  // namespace soil::stage::scene::gui::container
