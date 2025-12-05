#ifndef SOIL_VIDEO_EVENT_WINDOW_EVENT_H
#define SOIL_VIDEO_EVENT_WINDOW_EVENT_H
#include <cstdint>

#include "event/event.hpp"
#include "event/handler.hpp"

namespace soil::video {
class Window;
}
namespace soil::video::event {
struct WindowEvent final : soil::event::Event {
  enum CauseType : std::uint8_t { SizeChanged };

  CauseType Cause;
  class video::Window* Window;
  WindowEvent(soil::video::Window* Window, WindowEvent::CauseType Cause);
  ~WindowEvent() override = default;
};

using WindowEventHandler = soil::event::Handler<WindowEvent>;
}  // namespace soil::video::event
#endif
