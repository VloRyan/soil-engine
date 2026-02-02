#ifndef SOIL_ENGINE_STAGE_SCENE_GUI_LAYOUT_ANCHOR_H
#define SOIL_ENGINE_STAGE_SCENE_GUI_LAYOUT_ANCHOR_H
#include <glm/glm.hpp>
#include "alignment.hpp"

namespace soil::stage::scene::gui::layout {
class Anchor {
 public:
  Anchor() = default;
  Anchor(const Alignment& alignment);
  virtual ~Anchor() = default;
  [[nodiscard]] glm::vec3 Align(const glm::vec3& pos, const glm::ivec2& size,
                                const glm::ivec2& parentSize,
                                const glm::ivec4& padding) const;

  [[nodiscard]] const Alignment& GetAlignment() const;
  void SetAlignment(const struct Alignment& alignment);

  [[nodiscard]]static glm::vec3 Align(const Alignment& alignment, const glm::vec3& pos, const glm::ivec2& size,
                                      const glm::ivec2& parentSize,
                                      const glm::ivec4& padding);
 private:
  Alignment alignment_;
};
}  // namespace soil::stage::scene::gui::layout
#endif
