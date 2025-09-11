
#include "stage/scene/input.h"

namespace soil::stage::scene {
    Input::Input() : Node(Type::Input) { /*, {ReceiverType::Input}*/
        SetReceiverType(ReceiverType::Input, true);
    }

    input::EventMap& Input::GetEventMap() {
        return eventMap_;
    }

    void Input::Handle(const input::Event& event) {
        Node::Handle(event);
        eventMap_.Handle(event);
    }
} // namespace soil::stage::scene
