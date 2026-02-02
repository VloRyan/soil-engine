#ifndef SOIL_EXAMPLES_COMMON_COMPONENT_SHAPE_TILE_H
#define SOIL_EXAMPLES_COMMON_COMPONENT_SHAPE_TILE_H

#include "stage/scene/component/render/mesh_component.h"
#include "shape_component.hpp"
#include "prefab.hpp"

namespace soil_samples::common::component {
class ShapeTile : public ShapeComponent, public soil::stage::scene::component::render::MeshComponent {
 public:
  struct PrefabData {
    soil::video::vertex::Vao* QuadVao{nullptr};
    soil::video::mesh::Data* MeshData{nullptr};
    soil::video::shader::Program* Shader{nullptr};
    soil::video::texture::Texture* Texture{nullptr};
  };

  explicit ShapeTile(const std::string& prefab = "shapeTileDefault", bool isOpaque = true);

  ~ShapeTile() override = default;

  static Prefab<PrefabData> PREFABS;

  [[nodiscard]] virtual int GetTileIndex() const;
  virtual void SetTileIndex(int index);
  void SetPositionOffset(glm::vec3 offset);
  [[nodiscard]] glm::vec3 GetPositionOffset() const;

  [[nodiscard]] virtual glm::vec2 GetTileScale() const;
  virtual void SetTileScale(glm::vec2 scale);
  float GetRoundingRadius() const;
  void SetRoundingRadius(float rounding);

  void BeforeDrawElements(soil::video::render::State& state) override;
  float DistanceTo(const glm::vec3& point) override;
  bool IsVisible() const override;
  void SetVisible(bool visible) override;

  [[nodiscard]]  glm::vec2 GetSize() const override;
  void SetSize(const glm::vec2& size) override;
  [[nodiscard]]  glm::vec4 GetColor() const override;
  void SetColor(const glm::vec4& color) override;
  void SetFeature(Features feature, const FeatureValue& value) override;
  FeatureValue GetFeature(Features feature) override;

  inline static const auto SHADER_NAME = std::string("ShapeTile");

 private:
  const PrefabData& data_;
  glm::vec2 size_;
  glm::vec4 color_;
  int tileIndex_;
  glm::vec2 tileScale_;
  glm::vec3 positionOffset_;
  float roundingRadius_;
  bool effectiveVisible_;
};
}  // namespace soil_samples::common::component

#endif
