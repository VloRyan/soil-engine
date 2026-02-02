#include "stage/scene/gui/layout/anchor.h"

#include "stage/scene/gui/rectangle.h"
namespace soil::stage::scene::gui::layout {

Anchor::Anchor(const Alignment& alignment) : alignment_(alignment) {}

glm::vec3 Anchor::Align(const Alignment& alignment, const glm::vec3& pos, const glm::ivec2& size,
                        const glm::ivec2& parentSize,
                        const glm::ivec4& padding) {
  auto parentPadding = glm::vec4(padding);
  if (alignment.X == Alignment::Horizontal::None &&
      alignment.Y == Alignment::Vertical::None) {
    return pos;
  }
  auto alignedPos = pos;
  const auto parentHalfSize = glm::vec2(parentSize) / glm::vec2(2.F);
  const auto halfSize = glm::vec2(size) / glm::vec2(2.F);
  switch (alignment.X) {
    case Alignment::Horizontal::Left:
      alignedPos.x = -parentHalfSize.x + parentPadding[0] + halfSize.x;
      break;
    case Alignment::Horizontal::Right:
      alignedPos.x = parentHalfSize.x - parentPadding[2] - halfSize.x;
      break;
    case Alignment::Horizontal::Center:
      alignedPos.x = 0.F;
      break;
    case Alignment::Horizontal::None:
      break;
  }
  switch (alignment.Y) {
    case Alignment::Vertical::Top:
      alignedPos.y = parentHalfSize.y - parentPadding[1] - halfSize.y;
      break;
    case Alignment::Vertical::Bottom:
      alignedPos.y = -parentHalfSize.y + parentPadding[3] + halfSize.y;
      break;
    case Alignment::Vertical::Center:
      alignedPos.y = 0.F;
      break;
    case Alignment::Vertical::None:
      break;
  }
  return alignedPos;
}

glm::vec3 Anchor::Align(const glm::vec3& pos, const glm::ivec2& size,
                        const glm::ivec2& parentSize,
                        const glm::ivec4& padding) const {
  return Align(alignment_, pos, size, parentSize, padding);
}

const Alignment& Anchor::GetAlignment() const { return alignment_; }

void Anchor::SetAlignment(const Alignment& alignment) {
  alignment_ = alignment;
}
}  // namespace soil::stage::scene::gui::layout
