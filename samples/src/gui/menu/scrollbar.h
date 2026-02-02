#ifndef SOIL_EXAMPLES_GUI_SCROLLBAR_H
#define SOIL_EXAMPLES_GUI_SCROLLBAR_H
#include "common/node/pane.h"
#include "file/sprite_sheet.h"

namespace soil_samples::gui::menu {
class Scrollbar final : public common::node::Pane {
 public:
  struct ScrollerDefinition {
    std::string TileName{};
    Pane::Style Style{};
  };

  struct Definition {
    soil::file::SpriteSheet* SpriteSheet{nullptr};
    std::string BackgroundTileName{};
    ScrollerDefinition Scroller{};
  };
  enum class Direction : std::int8_t {
    Up = -1,
    Down = 1,
  };

  explicit Scrollbar(const Definition& definition);
  ~Scrollbar() override = default;

  void OnMouseButton(const glm::ivec2& pos, soil::input::MouseButton button,
                     soil::input::Event::StateType state) override;
  [[nodiscard]] float GetValue() const;
  void SetOnValueChange(const std::function<void(float)>& onValueChange);
  void SetValue(float value);
  void UpdateSize(const glm::ivec2& parentSize) override;
  [[nodiscard]] float GetMinValue() const;
  void SetMinValue(float minValue);
  [[nodiscard]] float GetMaxValue() const;
  void SetMaxValue(float maxValue);
  [[nodiscard]] float GetIncrement() const;
  void SetIncrement(float increment);

  void Scroll(Direction direction = Direction::Down);

 private:
  void UpdateScrollerPosition() const;
  Pane* scroller_;
  float value_;
  float minValue_;
  float maxValue_;
  float increment_;
  std::function<void(float)> onValueChange_;
};
}  // namespace soil_samples::gui::menu

#endif
