#include "stage/scene/component/mesh_component.h"

#include "stage/scene/node.h"

namespace soil::stage::scene::component {
MeshComponent::MeshComponent(const video::mesh::Data& mesh,
                             video::shader::Shader* shader, const bool opaque)
    : VisualComponent(render::Type::Plain, opaque),
      vertexArray_(new video::render::VertexArray(mesh)),
      shader_(shader) {}

MeshComponent::~MeshComponent() { delete vertexArray_; }

void MeshComponent::BuildVaoFrom(const video::mesh::Data& mesh) {
  vertexArray_ = new video::render::VertexArray(mesh);
}

video::render::VertexArray* MeshComponent::GetVertexArray() const {
  return vertexArray_;
}

video::render::RenderableObject* MeshComponent::GetRenderable() { return this; }

void MeshComponent::Render(video::render::State& state) {
  PrepareRender(state);
  shader_->Prepare(state);
  vertexArray_->Render(state);
}

float MeshComponent::DistanceTo(const glm::vec3& point) {
  return glm::distance(GetParent()->GetWorldPosition(), point);
}

video::shader::Shader* MeshComponent::GetShader() const { return shader_; }

}  // namespace soil::stage::scene::component
