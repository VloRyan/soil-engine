#ifndef BASIC_SHAPE_H
#define BASIC_SHAPE_H
#include <stage/resources.h>

#include "stage/scene/component/render/mesh_component.h"

namespace soil_samples::basic {

class Shape : public soil::stage::scene::component::render::MeshComponent {
 public:
  Shape(const soil::video::vertex::Vao* vao,
        soil::video::shader::Program* shader, bool isOpaque = true);
  ~Shape() override = default;
  [[nodiscard]] virtual glm::vec2 GetSize() const;
  virtual void SetSize(const glm::vec2& size);
  [[nodiscard]] virtual glm::vec4 GetColor() const;
  virtual void SetColor(const glm::vec4& color);
  [[nodiscard]] virtual byte GetTextureUnit() const;
  virtual void SetTextureUnit(byte textureUnit);

  void BeforeDrawElements(soil::video::render::State& state) override;
  float DistanceTo(const glm::vec3& point) override;

  inline static const std::string SHADER_NAME = "Basic";

 private:
  glm::vec2 size_;
  glm::vec4 color_;
  byte textureUnit_;
};
}  // namespace soil_samples::basic

#endif
