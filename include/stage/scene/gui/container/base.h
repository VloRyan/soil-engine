#ifndef SOIL_STAGE_SCENE_GUI_CONTAINER_BASE_H
#define SOIL_STAGE_SCENE_GUI_CONTAINER_BASE_H

#include "stage/scene/gui/rectangle.h"

namespace soil::stage::scene::gui::container {
class Base : public Rectangle {
 public:
  ~Base() override = default;

  [[nodiscard]] const std::vector<Rectangle*>& GetItems() const;
  [[nodiscard]] virtual const glm::ivec2& GetOffset() const;
  virtual void SetOffset(const glm::ivec2& offset);
  [[nodiscard]] virtual int GetMargin() const;
  virtual void SetMargin(int margin);
  void RemoveChild(Node* node) override;

  Rectangle* Child(int index);
  const glm::ivec2& GetScrollStep() const;
  void SetScrollStep(const glm::ivec2& scrollStep);

 protected:
  explicit Base(int margin = 0, glm::ivec4 padding = glm::ivec4(0),
                SizeTypes sizeType = SizeTypes::GrowWithContent);
  void addChildRect(Rectangle* rect) override;

  void OnMouseWheel(const glm::ivec2& pos, glm::vec2 offset) override;

  int margin_;
  glm::ivec2 offset_;
  glm::ivec2 scrollStep_;
};
}  // namespace soil::stage::scene::gui::container

#endif
