#ifndef SOIL_STAGE_EVENTS_NODE_EVENT_H
#define SOIL_STAGE_EVENTS_NODE_EVENT_H

#include "event/event.h"
#include "event/handler.hpp"

namespace soil::stage::scene::component {
    class Component;
} // namespace soil::stage::scene::component

namespace soil::stage::event {
    class Component final : soil::event::Event {
    public:
        enum class ChangeType {
            Added,
            Removed,
            State,
            UpdateType,
        };

        Component(scene::component::Component* origin, ChangeType type);

        ~Component() override = default;

        [[nodiscard]] ChangeType GetChangeType() const;

        [[nodiscard]] scene::component::Component* GetOrigin() const;

        friend bool operator==(const Component& lhs, const Component& rhs) {
            return static_cast<const soil::event::Event&>(lhs) == static_cast<const soil::event::Event&>(rhs) &&
                lhs.origin_ == rhs.origin_ &&
                lhs.changeType_ == rhs.changeType_ /*&& lhs.prevState_ == rhs.prevState_*/;
        }

        friend bool operator!=(const Component& lhs, const Component& rhs) {
            return !(lhs == rhs);
        }

        static Component MakeStateChangedEvent(scene::component::Component* origin);

        static Component MakeUpdateTypeChangedEvent(scene::component::Component* origin);

    private:
        scene::component::Component* origin_;
        ChangeType changeType_;
    };

    using ComponentEventHandler = soil::event::Handler<Component>;
} // namespace soil::stage::event
#endif
