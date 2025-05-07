#ifndef SOIL_WINDOW_EVENT_H
#define SOIL_WINDOW_EVENT_H
#include <cstdint>

#include "event/event.h"
#include "event/handler.hpp"

namespace soil {
    class Window;

    class WindowEvent final : public event::Event {
    public:
        enum Cause : std::uint8_t {
            SizeChanged,
            StatisticsChanged,
        };

        explicit WindowEvent(Window *window, Cause cause);

        ~WindowEvent() override;

        [[nodiscard]] Cause GetCause() const;

        [[nodiscard]] Window *GetWindow() const;

    private:
        Cause cause_;
        Window *window_;
    };

    using WindowEventHandler = event::Handler<WindowEvent>;
} // namespace soil
#endif /* SOIL_WINDOW_EVENT_H */
