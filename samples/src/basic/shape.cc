#include "shape.h"

#include <stage/scene/node.h>

#include <stdexcept>

namespace soil_samples::basic {
Shape::Shape(const soil::video::mesh::Data& mesh, const bool isOpaque,
             soil::video::shader::Shader* shader)
    : MeshComponent(mesh, shader, isOpaque),
      size_(1.F),
      color_(1.F),
      textureUnit_(0) {}

glm::vec2 Shape::GetSize() const { return size_; }

void Shape::SetSize(const glm::vec2& size) { size_ = size; }

glm::vec4 Shape::GetColor() const { return color_; }

void Shape::SetColor(const glm::vec4& color) { color_ = color; }

byte Shape::GetTextureUnit() const { return textureUnit_; }

void Shape::SetTextureUnit(const byte textureUnit) {
  this->textureUnit_ = textureUnit;
}

Shape* Shape::New(const soil::stage::Resources& resources, const bool isOpaque,
                  soil::video::shader::Shader* shader) {
  const auto* mesh =
      resources.GetMesh({.Identifier = "ShapeMesh",
                         .Type = soil::video::mesh::Prefab::Type::Quad});
  if (mesh == nullptr) {
    throw std::runtime_error("mesh is null");
  };
  return new Shape(*mesh, isOpaque, shader);
}

float Shape::DistanceTo(const glm::vec3& point) {
  return glm::distance(point.z, GetParent()->GetWorldPosition().z);
}

void Shape::PrepareRender(soil::video::render::State& state) {
  GetShader()->Use();
  GetShader()->SetUniform("Transform", GetParent()->GetWorldTransform());
  GetShader()->SetUniform("Size", GetSize());
  GetShader()->SetUniform("Color", GetColor());
  GetShader()->SetUniform("Texture", GetTextureUnit());
}
}  // namespace soil_samples::basic
