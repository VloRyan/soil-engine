#ifndef SOIL_STAGE_EVENT_NODE_H
#define SOIL_STAGE_EVENT_NODE_H

#include "component.h"
#include "event/event.h"

namespace soil::stage::scene {
    class Node;
}

namespace soil::stage::event {

    class Node final : soil::event::Event {
    public:
        enum class ChangeType : std::uint8_t {
            Deleted, // TODO implement
            State,
            ChildAdded,
            ChildRemoved,
            UpdateType,
            Component,
        };

        Node(scene::Node* origin, ChangeType type);

        ~Node() override;

        [[nodiscard]] ChangeType GetChangeType() const;

        [[nodiscard]] scene::Node* GetOrigin() const;

        [[nodiscard]] scene::Node* GetChangedNode() const;

        [[nodiscard]] const Component& GetComponentEvent() const;

        friend bool operator==(const Node& lhs, const Node& rhs) {
            return static_cast<const soil::event::Event&>(lhs) == static_cast<const soil::event::Event&>(rhs) &&
                lhs.origin_ == rhs.origin_ && lhs.changedNode_ == rhs.changedNode_ &&
                lhs.changeType_ == rhs.changeType_ && lhs.componentEvent_ == rhs.componentEvent_;
        }

        friend bool operator!=(const Node& lhs, const Node& rhs) {
            return !(lhs == rhs);
        }

        static Node MakeChildAddedEvent(scene::Node* origin, scene::Node* added);
        static Node MakeChildRemovedEvent(scene::Node* origin, scene::Node* removed);
        static Node MakeComponentEvent(scene::Node* origin, const Component& componentEvent);

    private:
        scene::Node* origin_;
        scene::Node* changedNode_;
        ChangeType changeType_;

        Component componentEvent_;
    };

    using NodeEventHandler = soil::event::Handler<Node>;
} // namespace soil::stage::event
#endif
