#include "stage/event/node.h"

#include "stage/event/component.h"

namespace soil::stage::event {
Node::Node(scene::Node* origin, enum ChangeType changeType,
           scene::Node* changedNode, scene::Node* prevParentNode,
           const Component& componentEvent)
    : Event(Type_t::Stage),
      Origin(origin),
      ChangeType(changeType),
      ChangedNode(changedNode),
      PrevParentNode(prevParentNode),
      ComponentEvent(componentEvent) {}

Node Node::MakeNodeDeletedEvent(scene::Node* origin) {
  return Node(origin, ChangeType::Deleted);
}

Node Node::MakeChildAddedEvent(scene::Node* origin, scene::Node* added) {
  return Node(origin, ChangeType::ChildAdded, added);
}

Node Node::MakeChildRemovedEvent(scene::Node* origin, scene::Node* removed) {
  return Node(origin, ChangeType::ChildRemoved, removed);
}

Node Node::MakeComponentEvent(scene::Node* origin,
                              const Component& componentEvent) {
  return Node(origin, ChangeType::Component, nullptr, nullptr, componentEvent);
}

}  // namespace soil::stage::event
