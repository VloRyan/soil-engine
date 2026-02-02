#ifndef SOIL_ENGINE_LES_SRC_COMMON_NODE_CHECKBOX_H
#define SOIL_ENGINE_LES_SRC_COMMON_NODE_CHECKBOX_H

#include "pane.h"
#include "file/sprite_sheet.h"
#include "common/component/shape_tile.h"

namespace soil_samples::common::node {
class Checkbox : public Pane {
 public:
  struct Style {
    soil::file::SpriteSheet* SpriteSheet{nullptr};
    std::string BackgroundTile;
    std::string CheckMarkTile;
  };
  Checkbox(bool checked, const Style& style);
  bool IsChecked() const;
  void SetChecked(bool checked);
  void UpdateSize(const glm::ivec2& maxSize) override;
  glm::ivec2 CalculateSize(const glm::ivec2& maxSize) override;
  void SetSize(const glm::ivec2& size) override;
  void SetOnChanged(const std::function<void()>& onChanged);
 protected:
  void OnMouseButton(const glm::ivec2& pos,
                     soil::input::MouseButton button,
                     soil::input::Event::StateType state) override;
  void BeforeNodeUpdate() override;
  void Layout() override;
  glm::ivec2 CalculateChildrenSize(const glm::ivec2& maxSize) override;
 private:
  bool checked_;
  component::ShapeTile* checkMark_;
  std::function<void()> onChanged_;
};
}  // namespace soil_samples::common::node
#endif
