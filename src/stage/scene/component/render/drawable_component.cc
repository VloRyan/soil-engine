#include "stage/scene/component/render/drawable_component.h"

namespace soil::stage::scene::component {
DrawableComponent::DrawableComponent(const bool opaque, const bool visible)
    : Component(Type::Drawable),
      visible_(visible),
      opaque_(opaque),
      culled_(false) {}

bool DrawableComponent::IsVisible() const { return visible_; }

void DrawableComponent::SetVisible(const bool visible) {
  if (visible_ == visible) {
    return;
  }
  visible_ = visible;
  SignalChanged(DrawableComponent::ChangeDetails::Visibility);
}

bool DrawableComponent::IsOpaque() const { return opaque_; }

void DrawableComponent::SetOpaque(const bool opaque) {
  if (opaque_ == opaque) {
    return;
  }
  opaque_ = opaque;
  SignalChanged(DrawableComponent::ChangeDetails::Opaque);
}

bool DrawableComponent::IsCulled() const { return culled_; }

void DrawableComponent::SetCulled(const bool culled) {
  if (culled_ == culled) {
    return;
  }
  culled_ = culled;
  SignalChanged(DrawableComponent::ChangeDetails::Visibility);
}

bool DrawableComponent::IsDrawablePile() { return false; }

void DrawableComponent::SignalChanged(DrawableComponent::ChangeDetails detail) {
  Component::SignalChanged(static_cast<int>(detail));
}

}  // namespace soil::stage::scene::component
