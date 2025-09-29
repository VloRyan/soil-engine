#include "stage/scene/component/component.h"
#include "stage/scene/node.h"

namespace soil::stage::scene::component {
    Component::Component(const Type type) :
        parent_(nullptr), type_(type), state_(State::Normal), updateType_(UpdateType::WhenDirty) {
    }

    Component::~Component() {
        if (parent_ != nullptr) {
            parent_->RemoveComponent(this);
        }
        // TODO: fire Removed event?! Currently only node creates and fires it
    }

    void Component::SetParent(Node* parent) {
        parent_ = parent;
    }

    void Component::SetUpdateType(const UpdateType updateType) {
        if (updateType_ == updateType) {
            return;
        }
        updateType_ = updateType;
        fire(event::Component::MakeUpdateTypeChangedEvent(this));
    }

    Component::UpdateType Component::GetUpdateType() const {
        return updateType_;
    }

    Component::Type Component::GetType() const {
        return type_;
    }

    void Component::Update() {
        SetState(State::Normal);
    }

    void Component::UpdateTransform(const glm::mat4& transform) {
    }

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
