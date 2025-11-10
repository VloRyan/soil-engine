#include "stage/scene/component/render/mesh_instance_component.h"

#include "stage/scene/node.h"
namespace soil::stage::scene::component::render {
MeshInstanceComponent::MeshInstanceComponent(
    const video::render::MeshInstancePile::PileDescriptor& pileDescriptor,
    const bool opaque)
    : RenderableComponent(opaque),
      instance_(new video::render::MeshInstance(
          pileDescriptor, [this](const video::render::data::IWriter& writer,
                                 soil::video::render::State& state) {
            ApplyData(writer, state);
          })) {}

MeshInstanceComponent::~MeshInstanceComponent() { delete instance_; }

video::render::Renderable* MeshInstanceComponent::GetRenderable() {
  return instance_;
}

float MeshInstanceComponent::DistanceTo(const glm::vec3& point) {
  return glm::distance(GetParent()->GetPosition(), point);
}

void MeshInstanceComponent::SetOpaque(bool opaque) {
  if (opaque == IsOpaque()) {
    return;
  }
  auto stateId = instance_->StateId();
  stateId.State.Blend = !opaque;
  instance_->SetStateId(stateId);
  RenderableComponent::SetOpaque(opaque);
}
}  // namespace soil::stage::scene::component::render
