#ifndef SOIL_INPUT_EVENT_MAP_H_
#define SOIL_INPUT_EVENT_MAP_H_

#include <functional>
#include <vector>
#include "constants.hpp"
#include "event.h"

namespace soil::input {
    struct KeyEventMapping {
        KeyEventMapping(Keys key, Event::State state, std::function<void(const Event &)> action);

        [[nodiscard]] bool Matches(const Event &event) const;

        Keys key_;
        Event::State state_;
        std::function<void(const Event &)> fun_;
    };

    struct MouseButtonEventMapping {
        MouseButtonEventMapping(MouseButton button, Event::State state, std::function<void(const Event &)> action);

        [[nodiscard]] bool Matches(const Event &event) const;

        MouseButton button_;
        Event::State state_;
        std::function<void(const Event &)> fun_;
    };

    struct MouseWheelEventMapping {
        explicit MouseWheelEventMapping(MouseWheelDirection direction, std::function<void(const Event &)> action);

        [[nodiscard]] bool Matches(const Event &event) const;
        MouseWheelDirection direction_;
        std::function<void(const Event &)> fun_;
    };

    class EventMap final : public EventHandler {
    public:
        explicit EventMap();

        ~EventMap() override;

        void Handle(const Event &event) override;

        EventMap &AddKeyMapping(Keys key, Event::State state, const std::function<void(const Event &)> &action);

        EventMap &AddMouseButtonMapping(MouseButton button, Event::State state,
                                        const std::function<void(const Event &)> &action);

        EventMap &AddMouseWheelMapping(MouseWheelDirection direction, const std::function<void(const Event &)> &action);

    private:
        std::vector<KeyEventMapping> keyMappings_;
        std::vector<MouseButtonEventMapping> mouseButtonMappings_;
        std::vector<MouseWheelEventMapping> mouseWheelMappings_;
    };
} // namespace soil::input

#endif
