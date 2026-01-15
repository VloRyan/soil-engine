#include "stage/scene/component/render/mesh_instance_component.h"

#include "stage/scene/node.h"
namespace soil::stage::scene::component::render {
MeshInstanceComponent::MeshInstanceComponent(
    const video::render::draw::VaoElementsInstanced::PileDescriptor&
        pileDescriptor)
    : DrawableComponent(true),
      pile_(video::render::draw::VaoElementsInstanced::GetPile(
          pileDescriptor.Name)) {
  if (pile_ == nullptr) {
    // TODO pile is not scene related -> global
    pile_ = video::render::draw::VaoElementsInstanced::Prepare(
        pileDescriptor, {
                            .Blend = false,
                            .DepthFunc = video::render::DepthFunc::Less,
                        });
  }
  pile_->Insert(this);
}
MeshInstanceComponent::MeshInstanceComponent(const std::string& name)
    : DrawableComponent(true),
      pile_(video::render::draw::VaoElementsInstanced::GetPile(name)) {
  if (pile_ == nullptr) {
    throw std::runtime_error("pile " + name + " is not prepared");
  }
  pile_->Insert(this);
}

MeshInstanceComponent::~MeshInstanceComponent() { pile_->Remove(this); }

video::render::draw::Drawable* MeshInstanceComponent::Drawable() {
  return pile_;
}

void MeshInstanceComponent::SetOpaque(
    bool opaque) { /*NOOP:  MeshInstances are always opaque*/ }

void MeshInstanceComponent::SetVisible(bool visible) {
  if (visible_ == visible) {
    return;
  }
  visible_ = visible;
  if (visible_ && !IsCulled()) {
    pile_->Insert(this);
    if (pile_->Container() == nullptr) {
      DrawableComponent::SignalChanged(
          DrawableComponent::ChangeDetails::Visibility);
    }
  } else {
    pile_->Remove(this);
  }
}
void MeshInstanceComponent::SetCulled(bool culled) {
  if (culled_ == culled) {
    return;
  }
  culled_ = culled;
  if (culled_ || !IsVisible()) {
    pile_->Remove(this);
  } else {
    pile_->Insert(this);
    if (pile_->Container() == nullptr) {
      DrawableComponent::SignalChanged(
          DrawableComponent::ChangeDetails::Visibility);
    }
  }
}

bool MeshInstanceComponent::IsDrawablePile() { return true; }
void MeshInstanceComponent::SignalChanged(ChangeDetails detail) {
  if (IsVisible() && !IsCulled()) {
    pile_->SetDirty(this);
  }
}
}  // namespace soil::stage::scene::component::render
