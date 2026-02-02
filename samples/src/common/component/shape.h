#ifndef SOIL_EXAMPLES_COMMON_COMPONENT_SHAPE_H
#define SOIL_EXAMPLES_COMMON_COMPONENT_SHAPE_H

#include "shape_component.hpp"
#include "stage/scene/component/render/mesh_component.h"
#include "prefab.hpp"
namespace soil_samples::common::component {

class Shape : public ShapeComponent, public soil::stage::scene::component::render::MeshComponent {
 public:
  struct PrefabData {
    soil::video::vertex::Vao* QuadVao{nullptr};
    soil::video::mesh::Data* MeshData{nullptr};
    soil::video::shader::Program* Shader{nullptr};
    soil::video::texture::Texture* Texture{nullptr};
  };

  Shape(const std::string& prefab, bool isOpaque = true);
  ~Shape() override = default;

  static Prefab<PrefabData> PREFABS;

  [[nodiscard]] virtual glm::vec2 GetSize() const override;
  virtual void SetSize(const glm::vec2& size) override;
  [[nodiscard]] virtual glm::vec4 GetColor() const override;
  virtual void SetColor(const glm::vec4& color) override;
  [[nodiscard]] virtual byte GetTextureUnit() const;
  virtual void SetTextureUnit(byte textureUnit);

  void BeforeDrawElements(soil::video::render::State& state) override;
  float DistanceTo(const glm::vec3& point) override;

  bool IsVisible() const override;
  void SetVisible(bool visible) override;
  void SetFeature(Features feature, const FeatureValue& value) override;
  FeatureValue GetFeature(Features feature) override;

  inline static const std::string SHADER_NAME = "Shape";

 private:
  glm::vec2 size_;
  glm::vec4 color_;
  byte textureUnit_;
  bool effectiveVisible_;
};
}  // namespace soil_samples::basic

#endif
