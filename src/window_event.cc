#include "window_event.h"

namespace soil {
WindowEvent::WindowEvent(class Window *window, const CauseType cause)
    : Event(Type_t::Window), Cause(cause), Window(window) {}

WindowEvent::~WindowEvent() = default;

}  // namespace soil
