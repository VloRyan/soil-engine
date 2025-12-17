#include "shape.h"

#include <stage/scene/node.h>

#include <stdexcept>

namespace soil_samples::basic {
Shape::Shape(const soil::video::vertex::Vao* vao,
             soil::video::shader::Program* shader, bool isOpaque)
    : MeshComponent(vao, soil::video::render::DrawMode::TriangleStrip, shader,
                    isOpaque),
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

void Shape::BeforeDraw() {
  GetShader()->SetUniform("Transform", GetParent()->Transform().GetMatrix());
  GetShader()->SetUniform("Size", GetSize());
  GetShader()->SetUniform("Color", GetColor());
  GetShader()->SetUniform("Texture", GetTextureUnit());
}
float Shape::DistanceTo(const glm::vec3& point) {
  return GetParent()->GetPosition().z;  // sort by z
}
}  // namespace soil_samples::basic
