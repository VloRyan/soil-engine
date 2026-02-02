#include "shape.h"

#include <stage/scene/node.h>

#include "video/shader/program.h"
#include "prefab.hpp"

namespace soil_samples::common::component {
Prefab<Shape::PrefabData> Shape::PREFABS = {};

Shape::Shape(const std::string& prefab, const bool isOpaque)
    : MeshComponent(PREFABS.Get(prefab).QuadVao,
                    soil::video::render::DrawMode::TriangleStrip,
                    PREFABS.Get(prefab).Shader, isOpaque),
      size_(1.F),
      color_(1.F),
      effectiveVisible_(true),
      textureUnit_(
          PREFABS.Get(prefab).Texture != nullptr ? PREFABS.Get(prefab).Texture->GetSlot()
                                                 : 0) {}

glm::vec2 Shape::GetSize() const { return size_; }

void Shape::SetSize(const glm::vec2& size) { size_ = size; }

glm::vec4 Shape::GetColor() const { return color_; }

void Shape::SetColor(const glm::vec4& color) { color_ = color; }

byte Shape::GetTextureUnit() const { return textureUnit_; }

void Shape::SetTextureUnit(const byte textureUnit) {
  this->textureUnit_ = textureUnit;
}

void Shape::BeforeDrawElements(soil::video::render::State& state) {
  GetShader()->SetUniform("Transform", GetParent()->Transform().GetMatrix());
  GetShader()->SetUniform("Size", GetSize());
  GetShader()->SetUniform("Color", GetColor());
  GetShader()->SetUniform("Texture", GetTextureUnit());
}

float Shape::DistanceTo(const glm::vec3& point) {
  return GetParent()->GetPosition().z;  // sort by z
}

void Shape::SetFeature(ShapeComponent::Features feature, const ShapeComponent::FeatureValue& value) {
  switch (feature) {
    case ShapeComponent::Features::TextureUnit:
      SetTextureUnit(value.Int.value());
      break;

    default:;
  }
}

ShapeComponent::FeatureValue Shape::GetFeature(ShapeComponent::Features feature) {
  switch (feature) {
    case ShapeComponent::Features::TextureUnit:
      return {.Int=GetTextureUnit()};
    default:;
  }
  return {};
}

void Shape::SetVisible(bool visible) {
  MeshComponent::SetVisible(visible);
  auto newEffectiveVisible = MeshComponent::IsVisible() && color_.a > 0.F;
  if (effectiveVisible_ != newEffectiveVisible) {
    effectiveVisible_ = newEffectiveVisible;
    SignalChanged(soil::stage::scene::component::DrawableComponent::
                  ChangeDetails::Visibility);
  }
}

bool Shape::IsVisible() const {
  return effectiveVisible_;
}
}  // namespace soil_samples::basic
