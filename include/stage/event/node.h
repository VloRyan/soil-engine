#ifndef ENGINE_STAGE_EVENT_NODE_H
#define ENGINE_STAGE_EVENT_NODE_H

#include "event/event.h"

namespace stage::scene {
    class Node;
}

namespace stage::event {
    class Node : ::event::Event {
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

        [[nodiscard]] scene::Node* GetOrigin() const;

        [[nodiscard]] scene::Node* GetChangedNode() const;

        Node &WithNode(scene::Node *node);

    private:
        scene::Node *origin_;
        scene::Node *changedNode_;
        ChangeType changeType_;
    };

    // using NodeEventHandler = ::event::Handler<Node>;
} // stage::event
#endif //ENGINE_STAGE_EVENT_NODE_H
