#ifndef SOIL_WINDOW_EVENT_H
#define SOIL_WINDOW_EVENT_H
#include <cstdint>

#include "event/event.hpp"
#include "event/handler.hpp"

namespace soil {
class Window;

struct WindowEvent final : event::Event {
  enum CauseType : std::uint8_t {
    SizeChanged,
    StatisticsChanged,
  };

  explicit WindowEvent(class Window* window, CauseType cause);

  ~WindowEvent() override;

  CauseType Cause;
  class Window* Window;
};

using WindowEventHandler = event::Handler<WindowEvent>;
}  // namespace soil
#endif
