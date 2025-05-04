#ifndef ENGINE_ENGINE_INPUT_EVENT_MAP_H_
#define ENGINE_ENGINE_INPUT_EVENT_MAP_H_

#include <vector>
#include <functional>
#include "constants.hpp"
#include "event.h"


namespace input {
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
        MouseWheelEventMapping(std::function<void(const Event &)> action);

        [[nodiscard]] static bool Matches(const Event &event);

        // MouseButton button_;
        //Event::State state_;
        std::function<void(const Event &)> fun_;
    };

    class EventMap : public EventHandler {
    public:
        explicit EventMap();

        ~EventMap() override;

        void Handle(const Event &event) override;

        void AddKeyMapping(Keys key, Event::State state, const std::function<void(const Event &)> &action);

        void AddMouseButtonMapping(MouseButton button, Event::State state,
                                   const std::function<void(const Event &)> &action);

        void AddMouseWheelMapping(
            const std::function<void(const Event &)> &action);

    private:
        std::vector<KeyEventMapping> keyMappings_;
        std::vector<MouseButtonEventMapping> mouseButtonMappings_;
        std::vector<MouseWheelEventMapping> mouseWheelMappings_;
    };
} // input

#endif //ENGINE_ENGINE_INPUT_EVENT_MAP_H_
