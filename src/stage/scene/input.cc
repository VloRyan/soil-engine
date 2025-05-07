
#include "stage/scene/input.h"
namespace soil::stage::scene {
    Input::Input(Node *parent) : Node(parent, Type::Input, {ReceiverType::Input}) {}

    input::EventMap &Input::GetEventMap() { return eventMap_; }

    void Input::Handle(const input::Event &event) {
        Node::Handle(event);
        eventMap_.Handle(event);
    }
} // namespace soil::stage::scene
