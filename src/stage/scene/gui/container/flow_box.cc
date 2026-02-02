#include "stage/scene/gui/container/flow_box.h"
namespace soil::stage::scene::gui::container {

FlowBox::FlowBox(const int margin, const glm::ivec4 padding)
    : soil::stage::scene::gui::container::Base(margin, padding) {}

void FlowBox::Layout() {
  if (childRects_.empty()) {
    return;
  }
  auto maxChildSize = GetSize() - Paddings();
  const auto halfSize = glm::vec2(GetSize()) / glm::vec2(2.F);
  auto offset =
      glm::vec3(static_cast<float>(padding_[0] + GetOffset().x) - halfSize.x,
                halfSize.y - static_cast<float>(GetOffset().y + padding_[1]),
                Rectangle::LAYER_Z_INCREMENT);
  auto rowSize = glm::ivec2(0);
  int rows = 0;
  for (auto* child : childRects_) {
    auto childSize = child->GetSize();
    auto halfItemSize = glm::vec2(childSize) / glm::vec2(2.F);
    if (rowSize.x + childSize.x > maxChildSize.x) {
      offset.y -= static_cast<float>(rowSize.y) +
          static_cast<float>( margin_);
      rowSize = glm::ivec2(0);
      rows++;
    }
    auto pos =
        offset + glm::vec3(halfItemSize.x + static_cast<float>(rowSize.x),
                           -halfItemSize.y, 0.F);
    child->SetLocalPosition(pos);
    rowSize.x += childSize.x + margin_;
    rowSize.y = std::max(rowSize.y, childSize.y);
  }
}

glm::ivec2 FlowBox::CalculateChildrenSize(const glm::ivec2& maxSize) {
  glm::ivec2 size(0);
  if (childRects_.empty()) {
    return size;
  }
  auto maxChildSize = glm::clamp(maxSize, minSize_, maxSize_) - Paddings();
  auto rowSize = glm::ivec2(0);
  for (auto* child : childRects_) {
    auto childSize = child->CalculateSize(maxChildSize);
    if (rowSize.x + childSize.x + (rowSize.x == 0 ? 0 : margin_) >
        maxChildSize.x) {
      size.x = std::max(size.x, rowSize.x);
      size.y += rowSize.y + (size.y == 0 ? 0 : margin_);
      rowSize = glm::ivec2(0);
    }
    rowSize.x += childSize.x + (rowSize.x == 0 ? 0 : margin_);
    rowSize.y = std::max(rowSize.y, childSize.y);
  }
  size.x = std::max(size.x, rowSize.x);
  size.y += rowSize.y + (size.y == 0 ? 0 : margin_);
  return size;
}

glm::ivec2 FlowBox::CalculateAlignedChildrenSize(const glm::ivec2& maxSize) const {
  glm::ivec2 size(0);
  if (childRects_.empty()) {
    return size;
  }
  auto maxChildSize = glm::clamp(maxSize, minSize_, maxSize_) - Paddings();
  auto rowSize = glm::ivec2(0);
  for (auto* child : childRects_) {
    auto childSize = child->GetSize();
    if (rowSize.x + childSize.x + (rowSize.x == 0 ? 0 : margin_) >
        maxChildSize.x) {
      size.x = std::max(size.x, rowSize.x);
      size.y += rowSize.y + (size.y == 0 ? 0 : margin_);
      rowSize = glm::ivec2(0);
    }
    rowSize.x += childSize.x + (rowSize.x == 0 ? 0 : margin_);
    rowSize.y = std::max(rowSize.y, childSize.y);
  }
  size.x = std::max(size.x, rowSize.x);
  size.y += rowSize.y + (size.y == 0 ? 0 : margin_);
  return size;
}
}  // namespace soil::stage::scene::gui::container
