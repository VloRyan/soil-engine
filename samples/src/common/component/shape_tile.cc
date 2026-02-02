#include "shape_tile.h"

#include "stage/scene/gui/rectangle.h"
#include "stage/stage.h"
#include "video/shader/program.h"

namespace soil_samples::common::component {
Prefab<ShapeTile::PrefabData> ShapeTile::PREFABS = {};

ShapeTile::ShapeTile(const std::string& prefab, const bool isOpaque)
    : MeshComponent(PREFABS.Get(prefab).QuadVao,
                    soil::video::render::DrawMode::TriangleStrip,
                    PREFABS.Get(prefab).Shader, isOpaque),
      data_(PREFABS.Get(prefab)),
      size_(0.F),
      color_(1.F),
      tileIndex_(-1),
      tileScale_(1.F),
      positionOffset_(0.F),
      roundingRadius_(0.F),
      effectiveVisible_(true) {}

int ShapeTile::GetTileIndex() const { return tileIndex_; }

void ShapeTile::SetTileIndex(const int index) {
  if (tileIndex_ == index) {
    return;
  }
  tileIndex_ = index;
}

void ShapeTile::SetTileScale(const glm::vec2 scale) {
  if (tileScale_ == scale) {
    return;
  }
  tileScale_ = scale;
}

glm::vec2 ShapeTile::GetTileScale() const { return tileScale_; }

glm::vec2 ShapeTile::GetSize() const { return size_; }

void ShapeTile::SetSize(const glm::vec2& size) {
  if (size_ == size) {
    return;
  }
  size_ = size;
}

glm::vec4 ShapeTile::GetColor() const { return color_; }

void ShapeTile::SetColor(const glm::vec4& color) {
  if (color_ == color) {
    return;
  }
  auto visibilityChanged =
      color_.a != color.a && (color.a == 0.F || color_.a == 0.F);
  color_ = color;
  SetOpaque(color_.a == 1.0F && roundingRadius_ == 0.F);
  effectiveVisible_ = MeshComponent::IsVisible() && color_.a > 0.F;
  if (visibilityChanged) {
    SignalChanged(soil::stage::scene::component::DrawableComponent::
                  ChangeDetails::Visibility);
  }
}

void ShapeTile::SetVisible(bool visible) {
  MeshComponent::SetVisible(visible);
  auto newEffectiveVisible = MeshComponent::IsVisible() && color_.a > 0.F;
  if (effectiveVisible_ != newEffectiveVisible) {
    effectiveVisible_ = newEffectiveVisible;
    SignalChanged(soil::stage::scene::component::DrawableComponent::
                  ChangeDetails::Visibility);
  }
}

bool ShapeTile::IsVisible() const {
  return effectiveVisible_;
}

void ShapeTile::SetPositionOffset(glm::vec3 offset) {
  if (offset == positionOffset_) {
    return;
  }
  positionOffset_ = offset;
}

glm::vec3 ShapeTile::GetPositionOffset() const { return positionOffset_; }

float ShapeTile::DistanceTo(const glm::vec3& point) {
  return glm::distance(
      point.z, GetParent()->GetPosition().z + positionOffset_.z);  // sort by z
}

void ShapeTile::BeforeDrawElements(soil::video::render::State& state) {
  const auto* parentRect =
      dynamic_cast<soil::stage::scene::gui::Rectangle*>(GetParent());
  if (parentRect != nullptr) {
    state.SetScissorTest(true);
    state.SetScissor(parentRect->GetScissorRect());
  } else {
    state.SetScissorTest(false);
  }
  auto transform = GetParent()->Transform().GetMatrix();
  transform[3] += glm::vec4(positionOffset_, 0.0F);
  GetShader()->SetUniform("uTransform", transform);
  GetShader()->SetUniform("uSize", size_);
  GetShader()->SetUniform("uTileScale", tileScale_);
  GetShader()->SetUniform("uTexture", data_.Texture->GetSlot());
  GetShader()->SetUniform("uColor", color_);
  GetShader()->SetUniform("uTileIndex", tileIndex_);
  GetShader()->SetUniform("uRadius", roundingRadius_);
}

float ShapeTile::GetRoundingRadius() const { return roundingRadius_; }

void ShapeTile::SetRoundingRadius(const float rounding) {
  roundingRadius_ = rounding;
  SetOpaque(color_.a == 1.0F && roundingRadius_ == 0.F);
}

void ShapeTile::SetFeature(ShapeComponent::Features feature, const ShapeComponent::FeatureValue& value) {
  switch (feature) {
    case ShapeComponent::Features::TileIndex:
      SetTileIndex(value.Int.value());
      break;
    case ShapeComponent::Features::TileScale:
      SetTileScale(value.Vec2.value());
      break;
    case ShapeComponent::Features::RoundingRadius:
      SetRoundingRadius(value.Float.value());
      break;
    default:;
  }
}

ShapeComponent::FeatureValue ShapeTile::GetFeature(ShapeComponent::Features feature) {
  switch (feature) {
    case ShapeComponent::Features::TileIndex:
      return {.Int=tileIndex_};
    case ShapeComponent::Features::TileScale:
      return {.Vec2=tileScale_};
    case ShapeComponent::Features::RoundingRadius:
      return {.Float=roundingRadius_};
    default:;
  }
  return {};
}

}  // namespace soil_samples::common::component
