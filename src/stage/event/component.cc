#include "stage/event/component.h"

namespace soil::stage::event {
    Component::Component(scene::component::Component *origin, const ChangeType type) :
        Event(Type::Stage), origin_(origin), changeType_(type) {}

    Component::ChangeType Component::GetChangeType() const { return changeType_; }

    scene::component::Component *Component::GetOrigin() const { return origin_; }
} // namespace soil::stage::event
