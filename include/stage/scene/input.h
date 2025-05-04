
#ifndef ENGINE_STAGE_SCENE_INPUT_H
#define ENGINE_STAGE_SCENE_INPUT_H

#include "node.h"
#include "input/event_map.h"

namespace stage::scene {
    class Input : public Node {
    public:
        explicit Input(Node *parent);

        [[nodiscard]] input::EventMap &GetEventMap();

        void Handle(const input::Event &event) override;

    private:
        input::EventMap eventMap_;
    };
}

#endif //ENGINE_STAGE_SCENE_INPUT_H
