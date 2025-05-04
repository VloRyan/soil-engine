#include "stage/scene/component/component.h"

#include <stdexcept>

#include "stage/scene/node.h"

namespace stage::scene::component {
Component::~Component() {
  if (parent_ != nullptr) {
    parent_->RemoveComponent(this);
  }
}

Component::Component(const Type type)
    : parent_(nullptr), type_(type), state_(State::Normal) {}

void Component::SetParent(Node *parent) {
  if (parent_ == parent) {
    return;
  }
  if (parent_ != nullptr && parent != nullptr) {
    throw std::logic_error("Component already added to Node");
  }
  parent_ = parent;
}

Component::Type Component::GetType() const { return type_; }

void Component::Update() { SetState(State::Normal); }

void Component::UpdateMatrix(const glm::mat4 &matrix) {
  SetState(State::Dirty);
}

Component::State Component::GetState() const { return state_; }

std::string Component::GetName() const { return name_; }

void Component::SetName(const std::string &name) { name_ = name; }

Node *Component::GetParent() const { return parent_; }

void Component::SetState(const State state) {
  if (state == state_) {
    return;
  }
  state_ = state;
  if (parent_ != nullptr) {
    const auto evnt =
        event::Component(this, event::Component::ChangeType::State);
    parent_->Handle(evnt);
  }
}

void Component::SetDirty() { SetState(State::Dirty); }

bool Component::IsDirty() const { return GetState() == State::Dirty; }
} // namespace stage::scene::component
