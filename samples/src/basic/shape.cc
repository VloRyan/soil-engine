#include "shape.h"

#include <stage/scene/node.h>
#include <stdexcept>

namespace soil_samples::basic {
    Shape::Shape(const soil::video::mesh::Data& mesh, const bool isOpaque, soil::video::shader::Shader* shader) :
        MeshComponent(mesh, shader, isOpaque), texture_(nullptr), size_(1.F), color_(1.F) {}

    soil::video::texture::Texture* Shape::GetTexture() const {
        return texture_;
    }

    void Shape::SetTexture(soil::video::texture::Texture* const texture) {
        texture_ = texture;
    }

    glm::vec2 Shape::GetSize() const {
        return size_;
    }

    void Shape::SetSize(const glm::vec2& size) {
        size_ = size;
    }

    glm::vec4 Shape::GetColor() const {
        return color_;
    }

    void Shape::SetColor(const glm::vec4& color) {
        color_ = color;
    }

    Shape* Shape::New(const soil::stage::Resources& resources, const bool isOpaque,
                      soil::video::shader::Shader* shader) {
        const auto* mesh =
            resources.GetMesh({.Identifier = "ShapeMesh", .Type = soil::video::mesh::Prefab::Type::Quad});
        if (mesh == nullptr) {
            throw std::runtime_error("mesh is null");
        };
        return new Shape(*mesh, isOpaque, shader);
    }

    void Shape::PrepareRender() {
        GetShader()->Use();
        GetShader()->SetUniform("Transform", GetParent()->GetWorldTransform());
        GetShader()->SetUniform("Size", GetSize());
        GetShader()->SetUniform("Color", GetColor());
        GetShader()->SetTexture2d(0, texture_, "Texture");
    }
} // namespace soil_samples::basic
