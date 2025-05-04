#ifndef ENGINE_STAGE_EVENTS_NODE_EVENT_H
#define ENGINE_STAGE_EVENTS_NODE_EVENT_H

#include "event/event.h"
#include "event/handler.hpp"
#include "stage/scene/component/component.h"

namespace stage::event {
    class Component : ::event::Event {
    public:
        enum class ChangeType {
            Added,
            Removed,
            State,
        };

        Component(scene::component::Component *origin, ChangeType type);

        ~Component() override = default;

        [[nodiscard]] ChangeType GetChangeType() const;

        [[nodiscard]] scene::component::Component* GetOrigin() const;

    private:
        scene::component::Component *origin_;
        ChangeType changeType_;
    };

    using ComponentEventHandler = ::event::Handler<Component>;
} // stage::event
#endif //ENGINE_STAGE_EVENTS_NODE_EVENT_H
