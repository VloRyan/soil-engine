#include "stage/event/node.h"

#include "stage/event/component.h"

namespace soil::stage::event {
Node::Node(scene::Node* origin, const ChangeType type)
    : Event(Type::Stage),
      origin_(origin),
      changedNode_(nullptr),
      changeType_(type),
      componentEvent_(nullptr, Component::ChangeType::State) {}

Node::~Node() = default;

Node::ChangeType Node::GetChangeType() const { return changeType_; }

scene::Node* Node::GetOrigin() const { return origin_; }

scene::Node* Node::GetChangedNode() const { return changedNode_; }

const Component& Node::GetComponentEvent() const { return componentEvent_; }

Node Node::MakeChildAddedEvent(scene::Node* origin, scene::Node* added) {
  auto evnt = Node(origin, ChangeType::ChildAdded);
  evnt.changedNode_ = added;
  return evnt;
}

Node Node::MakeChildRemovedEvent(scene::Node* origin, scene::Node* removed) {
  auto evnt = Node(origin, ChangeType::ChildRemoved);
  evnt.changedNode_ = removed;
  return evnt;
}

Node Node::MakeComponentEvent(scene::Node* origin,
                              const Component& componentEvent) {
  auto evnt = Node(origin, ChangeType::Component);
  evnt.componentEvent_ = componentEvent;
  return evnt;
}
}  // namespace soil::stage::event
