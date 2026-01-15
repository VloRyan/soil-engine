#ifndef SOIL_STAGE_SCENE_GUI_CONTAINER_V_BOX_H
#define SOIL_STAGE_SCENE_GUI_CONTAINER_V_BOX_H
#include "base.h"

namespace soil::stage::scene::gui::container {
/**
 * HBox orders items vertically (top to bottom).
 */
class VBox : public Base {
 public:
  explicit VBox(int margin = 0, glm::ivec4 padding = glm::ivec4(0));
  ~VBox() override = default;

  [[nodiscard]] virtual layout::Anchor::HorizontalAlignments GetItemAlignment()
      const;
  virtual void SetItemAlignment(layout::Anchor::HorizontalAlignments alignment);

 protected:
  void Layout() override;
  glm::ivec2 CalculateChildrenSize(const glm::ivec2& maxSize) override;

 private:
  layout::Anchor itemAnchor_;
};
}  // namespace soil::stage::scene::gui::container

#endif
