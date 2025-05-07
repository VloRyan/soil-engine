#include "stage/event/node.h"

namespace soil::stage::event {
    Node::Node(scene::Node *origin, const ChangeType type) :
        Event(Type::Stage), origin_(origin), changedNode_(nullptr), changeType_(type) {}

    Node::~Node() = default;

    Node::ChangeType Node::GetChangeType() const { return changeType_; }

    scene::Node *Node::GetOrigin() const { return origin_; }

    scene::Node *Node::GetChangedNode() const { return changedNode_; }

    Node &Node::WithNode(scene::Node *node) {
        changedNode_ = node;
        return *this;
    }
} // namespace soil::stage::event
