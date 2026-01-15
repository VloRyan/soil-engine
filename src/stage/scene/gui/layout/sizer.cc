#include "stage/scene/gui/layout/sizer.h"
namespace soil::stage::scene::gui::layout {

glm::ivec2 Sizer::CalculateRelativeSize(glm::vec2 relativeSize,
                                        float aspectRatio,
                                        const glm::ivec2& parentSize,
                                        const glm::ivec2& fallbackSize) {
  auto newSize = glm::ivec2(0);
  for (auto i = 0; i < 2; i++) {
    if (relativeSize[i] > 0.F) {
      newSize[i] =
          static_cast<int>(static_cast<float>(parentSize[i]) * relativeSize[i]);
    } else {
      newSize[i] = fallbackSize[i];
    }
  }
  if (aspectRatio != 0.F) {
    if (relativeSize.x > 0.F && relativeSize.y <= 0.F) {
      newSize.y = static_cast<int>(static_cast<float>(newSize.x) / aspectRatio);
    }
    if (relativeSize.y > 0.F && relativeSize.x <= 0.F) {
      newSize.x = static_cast<int>(static_cast<float>(newSize.y) * aspectRatio);
    }
  }
  return newSize;
}
}  // namespace soil::stage::scene::gui::layout
