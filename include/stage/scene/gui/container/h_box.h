#ifndef SOIL_ENGINE_STAGE_SCENE_GUI_CONTAINER_H_BOX_H
#define SOIL_ENGINE_STAGE_SCENE_GUI_CONTAINER_H_BOX_H
#include "base.h"

namespace soil::stage::scene::gui::container {
/**
 * HBox orders items horizontally (left to right).
 */
class HBox : public Base {
 public:
  explicit HBox(int margin = 0, glm::ivec4 padding = glm::ivec4(0));
  ~HBox() override = default;

 protected:
  void Layout() override;
  glm::ivec2 CalculateChildrenSize(const glm::ivec2& maxSize) override;
  glm::ivec2 CalculateAlignedChildrenSize(const glm::ivec2& maxSize) const override;
};
}  // namespace soil::stage::scene::gui::container

#endif
