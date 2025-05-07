#include "window_event.h"

namespace soil {
    WindowEvent::WindowEvent(Window *window, const Cause cause) : Event(Type::Window), cause_(cause), window_(window) {}

    WindowEvent::~WindowEvent() = default;

    WindowEvent::Cause WindowEvent::GetCause() const { return cause_; }

    Window *WindowEvent::GetWindow() const { return window_; }
} // namespace soil
