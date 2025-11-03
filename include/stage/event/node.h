#ifndef SOIL_STAGE_EVENT_NODE_H
#define SOIL_STAGE_EVENT_NODE_H

#include "component.h"
#include "event/event.hpp"

namespace soil::stage::scene {
class Node;
}

namespace soil::stage::event {
struct Node final : soil::event::Event {
 public:
  enum class ChangeType : std::uint8_t {
    Deleted,
    State,
    ChildAdded,
    ChildRemoved,
    UpdateType,
    Component,
  };

  explicit Node(scene::Node* origin = nullptr,
                enum ChangeType changeType = ChangeType::State,
                scene::Node* changedNode = nullptr,
                scene::Node* prevParentNode = nullptr,
                const Component& componentEvent = Component());
  ~Node() override = default;

  friend bool operator==(const Node& lhs, const Node& rhs) {
    return static_cast<const soil::event::Event&>(lhs) ==
               static_cast<const soil::event::Event&>(rhs) &&
           lhs.Origin == rhs.Origin && lhs.ChangedNode == rhs.ChangedNode &&
           lhs.PrevParentNode == rhs.PrevParentNode &&
           lhs.ChangeType == rhs.ChangeType &&
           lhs.ComponentEvent == rhs.ComponentEvent;
  }

  friend bool operator!=(const Node& lhs, const Node& rhs) {
    return !(lhs == rhs);
  }
  static Node MakeNodeDeletedEvent(scene::Node* origin);
  static Node MakeChildAddedEvent(scene::Node* origin, scene::Node* added);

  static Node MakeChildRemovedEvent(scene::Node* origin, scene::Node* removed);

  static Node MakeComponentEvent(scene::Node* origin,
                                 const Component& componentEvent);

  scene::Node* Origin{nullptr};
  ChangeType ChangeType{ChangeType::State};
  scene::Node* ChangedNode{nullptr};
  scene::Node* PrevParentNode{nullptr};
  Component ComponentEvent{nullptr};
};

using NodeEventHandler = soil::event::Handler<Node>;
}  // namespace soil::stage::event
#endif
