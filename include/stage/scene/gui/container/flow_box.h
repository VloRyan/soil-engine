#ifndef SOIL_STAGE_SCENE_GUI_CONTAINER_FLOW_BOX_H
#define SOIL_STAGE_SCENE_GUI_CONTAINER_FLOW_BOX_H
#include "base.h"
namespace soil::stage::scene::gui::container {
class FlowBox : public Base {
 public:
  FlowBox(int margin = 0, glm::ivec4 padding = glm::ivec4(0));
  virtual ~FlowBox() override = default;

 protected:
  void Layout() override;
  glm::ivec2 CalculateChildrenSize(const glm::ivec2& maxSize) override;
};
}  // namespace soil::stage::scene::gui::container
#endif
