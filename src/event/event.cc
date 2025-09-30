#include "event/event.h"

namespace soil::event {
Event::Event(const Type type) : type_(type) {}

Event::Event(const Event& orig) = default;

Event::~Event() = default;

Event::Type Event::GetType() const { return type_; }
}  // namespace soil::event
