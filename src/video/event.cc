#include "video/event.h"

namespace soil::video {
    Event::Event(const Cause cause) : event::Event(Type::Video), cause_(cause) {}

    Event::~Event() = default;

    Event::Cause Event::GetCause() const { return cause_; }
} // namespace soil::video
