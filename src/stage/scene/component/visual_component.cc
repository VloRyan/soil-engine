#include "stage/scene/component/visual_component.h"

namespace soil::stage::scene::component {
VisualComponent::VisualComponent(const render::Type renderType,
                                 const bool opaque, const bool visible)
    : Component(Type::Visual),
      renderType_(renderType),
      visible_(visible),
      opaque_(opaque),
      culled_(false) {}

render::Type VisualComponent::GetRenderType() const { return renderType_; }

bool VisualComponent::IsVisible() const { return visible_; }

void VisualComponent::SetVisible(const bool visible) {
  if (visible_ == visible) {
    return;
  }
  visible_ = visible;
  SetDirty();
}

bool VisualComponent::IsOpaque() const { return opaque_; }

void VisualComponent::SetOpaque(const bool opaque) {
  if (opaque_ == opaque) {
    return;
  }
  opaque_ = opaque;
  SetDirty();
}

bool VisualComponent::IsCulled() const { return culled_; }

void VisualComponent::SetCulled(const bool culled) {
  if (culled_ == culled) {
    return;
  }
  culled_ = culled;
  SetDirty();
}

VisualComponent* VisualComponent::Cast(Component* component) {
  if (component == nullptr || component->GetType() != Type::Visual) {
    return nullptr;
  }
  return dynamic_cast<VisualComponent*>(component);
}

bool VisualComponent::IsRenderType(Component* component,
                                   const render::Type renderType) {
  if (const auto* vComp = Cast(component);
      vComp == nullptr || vComp->GetRenderType() != renderType) {
    return false;
  }
  return true;
}
}  // namespace soil::stage::scene::component
