#include "stage/scene/gui/layout/anchor.h"

#include "stage/scene/gui/rectangle.h"
namespace soil::stage::scene::gui::layout {

Anchor::Anchor(const Alignment& alignment) : alignment_(alignment) {}

glm::vec3 Anchor::Align(const glm::vec3& pos, const glm::ivec2& size,
                        const glm::ivec2& parentSize,
                        const glm::ivec4& padding) const {
  auto parentPadding = glm::vec4(padding);
  if (alignment_.Horizontal == HorizontalAlignments::None &&
      alignment_.Vertical == VerticalAlignments::None) {
    return pos;
  }
  auto alignedPos = pos;
  const auto parentHalfSize = glm::vec2(parentSize) / glm::vec2(2.F);
  const auto halfSize = glm::vec2(size) / glm::vec2(2.F);
  switch (alignment_.Horizontal) {
    case HorizontalAlignments::Left:
      alignedPos.x = -parentHalfSize.x + parentPadding[0] + halfSize.x;
      break;
    case HorizontalAlignments::Right:
      alignedPos.x = parentHalfSize.x - parentPadding[2] - halfSize.x;
      break;
    case HorizontalAlignments::Center:
    case HorizontalAlignments::None:
      break;
  }
  switch (alignment_.Vertical) {
    case VerticalAlignments::Top:
      alignedPos.y = parentHalfSize.y - parentPadding[1] - halfSize.y;
      break;
    case VerticalAlignments::Bottom:
      alignedPos.y = -parentHalfSize.y + parentPadding[3] + halfSize.y;
      break;
    case VerticalAlignments::Center:
    case VerticalAlignments::None:
      break;
  }
  return alignedPos;
}
const Anchor::Alignment& Anchor::GetAlignment() const { return alignment_; }
void Anchor::SetAlignment(const Alignment& alignment) {
  alignment_ = alignment;
}
}  // namespace soil::stage::scene::gui::layout
