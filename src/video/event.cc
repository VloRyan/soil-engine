#include "video/event.h"

namespace video
{
    Event::Event(const Event::Cause cause) : event::Event(event::Event::Type::Video), cause_(cause) {}

    Event::~Event() = default;

    Event::Cause Event::GetCause() const { return cause_; }
} // namespace video
