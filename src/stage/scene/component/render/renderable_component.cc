#include "stage/scene/component/render/renderable_component.h"

namespace soil::stage::scene::component {
RenderableComponent::RenderableComponent(const bool opaque, const bool visible)
    : Component(Type::Renderable),
      visible_(visible),
      opaque_(opaque),
      culled_(false) {}

bool RenderableComponent::IsVisible() const { return visible_; }

void RenderableComponent::SetVisible(const bool visible) {
  if (visible_ == visible) {
    return;
  }
  visible_ = visible;
  SignalChanged();
}

bool RenderableComponent::IsOpaque() const { return opaque_; }

void RenderableComponent::SetOpaque(const bool opaque) {
  if (opaque_ == opaque) {
    return;
  }
  opaque_ = opaque;
  SignalChanged();
}

bool RenderableComponent::IsCulled() const { return culled_; }

void RenderableComponent::SetCulled(const bool culled) {
  if (culled_ == culled) {
    return;
  }
  culled_ = culled;
  SignalChanged();
}
/*
RenderableComponent* RenderableComponent::Cast(Component* component) {
  if (component == nullptr || component->GetType() != Type::Visual) {
    return nullptr;
  }
  return dynamic_cast<RenderableComponent*>(component);
}

bool RenderableComponent::IsRenderType(Component* component,
                                       const render::Type renderType) {
  if (const auto* vComp = Cast(component);
      vComp == nullptr || vComp->GetRenderType() != renderType) {
    return false;
  }
  return true;
}*/
}  // namespace soil::stage::scene::component
