#include "event/event.h"

namespace soil::event {
    Event::Event(const Type type) : type_(type), consumed_(false) {}

    Event::Event(const Event &orig) = default;

    Event::~Event() = default;

    void Event::Consume() { consumed_ = true; }

    Event::Type Event::GetType() const { return type_; }

    bool Event::IsConsumed() const { return consumed_; }
} // namespace soil::event
