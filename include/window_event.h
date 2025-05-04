#ifndef  ENGINE_EVENT_SYSTEM_EVENT_H
#define  ENGINE_EVENT_SYSTEM_EVENT_H
#include <cstdint>

#include "event/event.h"
#include "event/handler.hpp"

namespace engine {
    class Window;

    class WindowEvent : public event::Event {
    public:
        enum Cause : std::uint8_t {
            SizeChanged,
            StatisticsChanged,
        };

        explicit WindowEvent(Window *window, Cause cause);

        ~WindowEvent() override;

        [[nodiscard]] Cause GetCause() const;

        [[nodiscard]] Window* GetWindow() const;

    private:
        Cause cause_;
        Window *window_;
    };

    using WindowEventHandler = event::Handler<WindowEvent>;
}
#endif /* ENGINE_EVENT_SYSTEM_EVENT_H */
