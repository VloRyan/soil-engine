#ifndef SOIL_STAGE_EVENT_NODE_H
#define SOIL_STAGE_EVENT_NODE_H

#include "event/event.h"

namespace soil::stage::scene {
    class Node;
}

namespace soil::stage::event {
    class Node final : soil::event::Event {
    public:
        enum class ChangeType : std::uint8_t {
            Created,
            Deleted,
            State,
            ChildAdded,
            ChildRemoved,
        };

        Node(scene::Node *origin, ChangeType type);

        ~Node() override;

        [[nodiscard]] ChangeType GetChangeType() const;

        [[nodiscard]] scene::Node *GetOrigin() const;

        [[nodiscard]] scene::Node *GetChangedNode() const;

        Node &WithNode(scene::Node *node);

    private:
        scene::Node *origin_;
        scene::Node *changedNode_;
        ChangeType changeType_;
    };

    // using NodeEventHandler = ::event::Handler<Node>;
} // namespace soil::stage::event
#endif // SOIL_STAGE_EVENT_NODE_H
