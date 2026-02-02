#ifndef SOIL_EXAMPLES_COMMON_COMPONENT_SHAPE_COMPONENT_H
#define SOIL_EXAMPLES_COMMON_COMPONENT_SHAPE_COMPONENT_H
#include "stage/scene/component/render/drawable_component.h"

namespace soil_samples::common::component {
class ShapeComponent {
 public:
  enum class Features : std::uint8_t {
    TextureUnit = 0,
    TileIndex,
    TileScale,
    RoundingRadius,
  };
  struct FeatureValue {
    std::optional<int> Int;
    std::optional<float> Float;
    std::optional<glm::vec2> Vec2;
  };
  [[nodiscard]] virtual glm::vec2 GetSize() const = 0;
  virtual void SetSize(const glm::vec2& size) = 0;
  [[nodiscard]] virtual glm::vec4 GetColor() const = 0;
  virtual void SetColor(const glm::vec4& color) = 0;

  [[nodiscard]] virtual bool IsVisible() const = 0;
  virtual void SetVisible(bool visible) = 0;

  void SetFeature(Features feature, int value) {
    SetFeature(feature, {.Int=value});
  }

  void SetFeature(Features feature, float value) {
    SetFeature(feature, {.Float=value});
  }

  void SetFeature(Features feature, const glm::vec2& value) {
    SetFeature(feature, {.Vec2=value});
  }

  virtual void SetFeature(Features feature, const FeatureValue& value) {}

  virtual FeatureValue GetFeature(Features feature) { return {}; }

};
}
#endif
