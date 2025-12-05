#include "video/event/window_event.h"
namespace soil::video::event {
WindowEvent::WindowEvent(soil::video::Window* Window,
                         WindowEvent::CauseType Cause)
    : Event(soil::event::Event::Type_t::Window), Window(Window), Cause(Cause) {}
}  // namespace soil::video::event
