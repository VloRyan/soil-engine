
#include "stage/scene/input.h"

stage::scene::Input::Input(Node *parent)
    : Node(parent, Type::Input, {ReceiverType::Input}) {}

input::EventMap &stage::scene::Input::GetEventMap() { return eventMap_; }

void stage::scene::Input::Handle(const input::Event &event) {
  Node::Handle(event);
  eventMap_.Handle(event);
}
