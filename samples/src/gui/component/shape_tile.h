#ifndef GUI_COMPONENT_SHAPE_TILE_H
#define GUI_COMPONENT_SHAPE_TILE_H

#include "stage/scene/component/render/mesh_component.h"

namespace soil_samples::gui::component {
class ShapeTile : public soil::stage::scene::component::render::MeshComponent {
 public:
  struct PrefabData {
    soil::video::vertex::Vao* QuadVao{nullptr};
    soil::video::mesh::Data* MeshData{nullptr};
    soil::video::shader::Program* Shader{nullptr};
    soil::video::texture::Texture* Texture{nullptr};
  };

  explicit ShapeTile(const std::string& prefab, bool isOpaque = true);

  ~ShapeTile() override = default;

  static void InitPrefab(const std::string& name, const PrefabData& data);

  [[nodiscard]] virtual int GetTileIndex() const;
  virtual void SetTileIndex(int index);
  [[nodiscard]] virtual glm::vec2 GetSize() const;
  virtual void SetSize(const glm::vec2& size);
  [[nodiscard]] virtual glm::vec4 GetColor() const;
  virtual void SetColor(const glm::vec4& color);
  void SetPositionOffset(glm::vec3 offset);
  [[nodiscard]] glm::vec3 GetPositionOffset() const;

  [[nodiscard]] virtual glm::vec2 GetTileScale() const;
  virtual void SetTileScale(glm::vec2 scale);

  void BeforeDraw() override;
  // void Apply(soil::video::render::State& state) override;
  float DistanceTo(const glm::vec3& point) override;
  // float Ordinal(const soil::video::render::State& state) override;
  void OnBind(soil::video::render::State& state) override;

 private:
  static std::unordered_map<std::string, PrefabData> PREFABS;
  PrefabData* data_;
  glm::vec2 size_;
  glm::vec4 color_;
  int tileIndex_;
  glm::vec2 tileScale_;
  glm::vec3 positionOffset_;
};
}  // namespace soil_samples::gui::component

#endif
