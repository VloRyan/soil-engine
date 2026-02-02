#ifndef SOIL_EXAMPLES_COMMON_NODE_PLANE_H
#define SOIL_EXAMPLES_COMMON_NODE_PLANE_H

#include "stage/scene/gui/rectangle.h"
#include "common/component/shape_component.hpp"

namespace soil_samples::common::node {
class Pane : public soil::stage::scene::gui::Rectangle {
 public:
  struct Style {
    glm::vec4 BackgroundColor{1.F};
    glm::vec4 BackgroundColorMouseOver{1.F};
  };

  explicit Pane(const std::string& backgroundPrefab = "shapeTileDefault");
  ~Pane() override;

  [[nodiscard]] virtual Style GetStyle() const;
  virtual void SetStyle(const Style& style);

  component::ShapeComponent& Background() const;

  virtual void SetOnClick(
      const std::function<void(soil::input::MouseButton button)>& onClick);

  Pane* GetToolTip() const;
  void SetToolTip(Pane* toolTip);

  static inline auto HoverStyle = Pane::Style{
      .BackgroundColor = glm::vec4(0.8F, 0.8F, 0.8F, 1.F),
      .BackgroundColorMouseOver = glm::vec4(1.F),
  };
  static inline auto ToolTipStyle = Pane::Style{
      .BackgroundColor = glm::vec4(0.5F, 0.5F, 0.5F, 0.8F),
      .BackgroundColorMouseOver = glm::vec4(0.5F, 0.5F, 0.5F, 0.8F),
  };

 protected:
  explicit Pane(const std::string& backgroundPrefab, const Style& style);
  void OnMouseOver(const glm::ivec2& pos) override;
  void OnMouseOut() override;
  void OnMouseButton(const glm::ivec2& pos, soil::input::MouseButton button,
                     soil::input::Event::StateType state) override;

  void OnParentChanged(Node* parent, Node* prevParent) override;
  void BeforeNodeUpdate() override;

 private:
  component::ShapeComponent* background_;
  Style style_;
  std::function<void(soil::input::MouseButton button)> onClick_;
  Pane* toolTip_;
};

}  // namespace soil_samples::common::node

#endif
