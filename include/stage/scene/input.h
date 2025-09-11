
#ifndef SOIL_STAGE_SCENE_INPUT_H
#define SOIL_STAGE_SCENE_INPUT_H

#include "input/event_map.h"
#include "node.h"

namespace soil::stage::scene {
    class Input final : public Node {
    public:
        explicit Input();

        [[nodiscard]] input::EventMap& GetEventMap();

        void Handle(const input::Event& event) override;

    private:
        input::EventMap eventMap_;
    };
} // namespace soil::stage::scene

#endif
