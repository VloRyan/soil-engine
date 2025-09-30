#ifndef GUI_PLANE_H
#define GUI_PLANE_H

#include "component/shape_tile.h"
#include "stage/scene/gui/rectangle.h"

namespace soil_samples::gui {
class Plane : public soil::stage::scene::gui::Rectangle {
 public:
  struct Style {
    glm::vec4 BackgroundColor{1.F};
    glm::vec4 BackgroundColorMouseOver{1.F};
  };

  explicit Plane(const std::string& tilePrefab = "gui");
  ~Plane() override = default;

  void SetSize(const glm::ivec2& size) override;
  void SetVisible(bool visible) override;

  [[nodiscard]] virtual Style GetStyle() const;
  virtual void SetStyle(const Style& style);

  component::ShapeTile& Background() const;
  const soil::video::render::Rect& GetScissorRect() const override;

  [[nodiscard]] virtual Rectangle* GetScissorFrom() const;
  virtual void SetScissorFrom(Rectangle* scissorRect);

 protected:
  explicit Plane(const std::string& tilePrefab, const Style& style);
  void OnMouseOver(const glm::ivec2& pos) override;
  void OnMouseOut() override;
  void BeforeNodeUpdate() override;

 private:
  component::ShapeTile* background_;
  Style style_;
  Rectangle* scissorFrom_;
};

}  // namespace soil_samples::gui

#endif
