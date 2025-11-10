#include "stage/scene/component/component.h"

#include "stage/scene/node.h"
#include "stage/scene/scene.h"

namespace soil::stage::scene::component {
Component::Component(const Type type)
    : parent_(nullptr), type_(type), updateType_(UpdateType::WhenNodeDirty) {}

Component::~Component() {
  if (parent_ != nullptr) {
    parent_->RemoveComponent(this);
  }
  // TODO: fire Removed event?! Currently only node creates and fires it
}

void Component::SetParent(Node* parent) {
  if (parent_ == parent) {
    return;
  }
  parent_ = parent;
  auto prevRoot = parent != nullptr ? parent->Root() : nullptr;
  auto prevStage = prevRoot != nullptr ? prevRoot->GetStage() : nullptr;
  auto root = parent_ != nullptr ? parent_->Root() : nullptr;
  auto stage = root != nullptr ? root->GetStage() : nullptr;
  OnStageChanged(stage, prevStage);
}

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
void Component::OnStageChanged(Stage* stage, Stage* prevStage) {}
}  // namespace soil::stage::scene::component
