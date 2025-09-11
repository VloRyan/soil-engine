#include "stage/scene/component/component.h"
#include "stage/scene/node.h"

namespace soil::stage::scene::component {
    Component::Component(const Type type) : parent_(nullptr), type_(type), state_(State::Normal) {}

    Component::~Component() {
        if (parent_ != nullptr) {
            parent_->RemoveComponent(this);
        }
    }

    void Component::SetParent(Node* parent) {
        parent_ = parent;
    }

    Component::Type Component::GetType() const {
        return type_;
    }

    void Component::Update() {
        SetState(State::Normal);
    }

    void Component::UpdateTransform(const glm::mat4& transform) {}

    Component::State Component::GetState() const {
        return state_;
    }

    Node* Component::GetParent() const {
        return parent_;
    }

    void Component::SetState(const State state) {
        if (state == state_) {
            return;
        }
        state_ = state;
        fire(event::Component::MakeStateChangedEvent(this));
    }

    void Component::SetDirty() {
        SetState(State::Dirty);
    }

    bool Component::IsDirty() const {
        return GetState() == State::Dirty;
    }
} // namespace soil::stage::scene::component
