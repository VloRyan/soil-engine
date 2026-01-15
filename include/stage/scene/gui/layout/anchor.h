#ifndef SOIL_ENGINE_STAGE_SCENE_GUI_LAYOUT_ANCHOR_H
#define SOIL_ENGINE_STAGE_SCENE_GUI_LAYOUT_ANCHOR_H
#include <glm/glm.hpp>

namespace soil::stage::scene::gui::layout {
class Anchor {
 public:
  enum class HorizontalAlignments { None = 0, Left, Center, Right };
  enum class VerticalAlignments { None = 0, Top, Center, Bottom };
  struct Alignment {
    HorizontalAlignments Horizontal{HorizontalAlignments::None};
    VerticalAlignments Vertical{VerticalAlignments::None};
    bool operator==(const Alignment& rhs) const {
      return Horizontal == rhs.Horizontal && Vertical == rhs.Vertical;
    }
    bool operator!=(const Alignment& rhs) const { return !(rhs == *this); }
  };

  Anchor() = default;
  Anchor(const Alignment& alignment);
  virtual ~Anchor() = default;
  [[nodiscard]] glm::vec3 Align(const glm::vec3& pos, const glm::ivec2& size,
                                const glm::ivec2& parentSize,
                                const glm::ivec4& padding) const;

  [[nodiscard]] const Alignment& GetAlignment() const;
  void SetAlignment(const struct Alignment& alignment);

 private:
  Alignment alignment_;
};
}  // namespace soil::stage::scene::gui::layout
#endif
