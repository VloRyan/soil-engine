#include "stage/scene/gui/container/v_box.h"

#include "stage/scene/gui/rectangle.h"
namespace soil::stage::scene::gui::container {

VBox::VBox(const int margin, const glm::ivec4 padding) : Base(margin, padding) {
}

void VBox::Layout() {
  if (childRects_.empty()) {
    return;
  }
  const auto halfSize = glm::vec2(GetSize()) / glm::vec2(2.F);
  auto offset = glm::vec3(GetOffset(), Rectangle::LAYER_Z_INCREMENT);
  switch (itemAlignment_.Y) {
    case layout::Alignment::Vertical::Top:
      offset.y += halfSize.y - static_cast<float>(padding_[1]);
      break;
    case layout::Alignment::Vertical::Bottom:
      offset.y -= halfSize.y - static_cast<float>(padding_[3]) - static_cast<float>(childrenSize_.y);
      break;
    case layout::Alignment::Vertical::Center:
    case layout::Alignment::Vertical::None:
      offset.y += static_cast<float>(childrenSize_.y) * 0.5F;
      break;
  }

  for (auto* child : childRects_) {
    if (!child->IsVisible()) {
      continue;
    }
    auto halfItemSize = glm::vec2(child->GetSize()) / glm::vec2(2.F);
    auto pos = offset + glm::vec3(0.F, -halfItemSize.y, 0.F);
    child->SetLocalPosition(layout::Anchor::Align({.X=itemAlignment_.X},
                                                  pos,
                                                  child->GetSize(),
                                                  GetSize(),
                                                  GetPadding()));
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

glm::ivec2 VBox::CalculateAlignedChildrenSize(const glm::ivec2& maxSize) const {
  glm::ivec2 size(minSize_.x, 0);
  if (childRects_.empty()) {
    return size;
  }
  for (auto* child : childRects_) {
    if (!child->IsVisible()) {
      continue;
    }
    auto childSize = child->GetSize();
    size.x = std::max(childSize.x, size.x);
    size.y += childSize.y;
  }
  size.y += (static_cast<int>(childRects_.size()) - 1) * margin_;
  return size;
}
}  // namespace soil::stage::scene::gui::container
