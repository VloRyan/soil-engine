#include "stage/scene/component/mesh_component.h"

#include "stage/scene/node.h"

namespace soil::stage::scene::component {
MeshComponent::MeshComponent(const video::mesh::Data& mesh,
                             video::shader::Shader* shader, const bool opaque)
    : RenderableComponent(opaque),
      mesh_(new video::render::MeshRenderable(
          mesh, shader,
          [this](const video::render::data::IWriter& writer,
                 soil::video::render::State& state) {
            ApplyData(writer, state);
          })),
      shader_(shader) {}

MeshComponent::~MeshComponent() { delete mesh_; }

video::render::Renderable* MeshComponent::GetRenderable() { return mesh_; }

float MeshComponent::DistanceTo(const glm::vec3& point) {
  return glm::distance(GetParent()->GetPosition(), point);
}

video::shader::Shader* MeshComponent::GetShader() const { return shader_; }

void MeshComponent::SetOpaque(bool opaque) {
  if (opaque == IsOpaque()) {
    return;
  }
  auto stateId = mesh_->StateId();
  stateId.State.Blend = !opaque;
  mesh_->SetStateId(stateId);
  RenderableComponent::SetOpaque(opaque);
}

}  // namespace soil::stage::scene::component
