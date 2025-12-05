#include "stage/scene/component/render/mesh_component.h"

#include "stage/scene/node.h"

namespace soil::stage::scene::component::render {
MeshComponent::MeshComponent(const video::vertex::Vao* vao,
                             video::render::DrawMode mode,
                             video::shader::Shader* shader, const bool opaque)
    : DrawableComponent(opaque),
      mesh_(new video::render::draw::VaoElements(
          vao, shader, mode, *this,
          {
              .Blend = !opaque,
              .DepthFunc = video::render::DepthFunc::Less,
          })) {}

MeshComponent::~MeshComponent() { delete mesh_; }

video::render::draw::Drawable* MeshComponent::Drawable() { return mesh_; }

float MeshComponent::DistanceTo(const glm::vec3& point) {
  return glm::distance(GetParent()->GetPosition(), point);
}

video::shader::Shader* MeshComponent::GetShader() const {
  return mesh_->StateId().Shader;
}

void MeshComponent::SetOpaque(bool opaque) {
  if (opaque == IsOpaque()) {
    return;
  }
  auto state = mesh_->StateId().State;
  state.Blend = !opaque;
  mesh_->UpdateState(state);
  DrawableComponent::SetOpaque(opaque);
}
void MeshComponent::BeforeDraw() {}
void MeshComponent::SignalChanged() { /*No need*/ }
void MeshComponent::SetVisible(bool visible) {
  if (visible_ == visible) {
    return;
  }
  visible_ = visible;
  DrawableComponent::SignalChanged();
}
void MeshComponent::SetCulled(bool culled) {
  if (culled_ == culled) {
    return;
  }
  culled_ = culled;
  DrawableComponent::SignalChanged();
}
}  // namespace soil::stage::scene::component::render
