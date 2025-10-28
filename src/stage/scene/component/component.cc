#include "stage/scene/component/component.h"

#include "stage/scene/node.h"

namespace soil::stage::scene::component {
Component::Component(const Type type)
    : parent_(nullptr), type_(type), updateType_(UpdateType::WhenNodeDirty) {}

Component::~Component() {
  if (parent_ != nullptr) {
    parent_->RemoveComponent(this);
  }
  // TODO: fire Removed event?! Currently only node creates and fires it
}

void Component::SetParent(Node* parent) { parent_ = parent; }

void Component::SetUpdateType(const UpdateType updateType) {
  if (updateType_ == updateType) {
    return;
  }
  updateType_ = updateType;
  fire(event::Component::MakeUpdateTypeChangedEvent(this));
}

Component::UpdateType Component::GetUpdateType() const { return updateType_; }

Component::Type Component::GetType() const { return type_; }

void Component::Update() {}

Node* Component::GetParent() const { return parent_; }

void Component::SignalChanged() {
  fire(event::Component::MakeDataChangedEvent(this));
}

}  // namespace soil::stage::scene::component
