#include "stage/event/component.h"

#include <glm/gtc/constants.hpp>

namespace soil::stage::event {
    Component::Component(scene::component::Component* origin, const ChangeType type) :
        Event(Type::Stage), origin_(origin), changeType_(type) {
    }

    Component::ChangeType Component::GetChangeType() const {
        return changeType_;
    }

    scene::component::Component* Component::GetOrigin() const {
        return origin_;
    }

    Component Component::MakeStateChangedEvent(scene::component::Component* origin) {
        return {origin, ChangeType::State};
    }

    Component Component::MakeUpdateTypeChangedEvent(scene::component::Component* origin) {
        return {origin, ChangeType::UpdateType};
    }
} // namespace soil::stage::event
