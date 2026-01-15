#ifndef SOIL_ENGINE_STAGE_SCENE_GUI_LAYOUT_SIZER_H
#define SOIL_ENGINE_STAGE_SCENE_GUI_LAYOUT_SIZER_H
#include <glm/glm.hpp>

namespace soil::stage::scene::gui::layout {
class Sizer {
 public:
  Sizer() = delete;
  static glm::ivec2 CalculateRelativeSize(glm::vec2 relativeSize,
                                          float aspectRatio,
                                          const glm::ivec2& parentSize,
                                          const glm::ivec2& fallbackSize);
};
}  // namespace soil::stage::scene::gui::layout
#endif
