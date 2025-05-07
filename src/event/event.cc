#include "event/event.h"

namespace soil::event {
    Event::Event(const Type type) : type_(type), consumed_(false) {}

    Event::Event(const Event &orig) = default;

    Event::~Event() = default;

    void Event::consume() { consumed_ = true; }

    Event::Type Event::getType() const { return type_; }

    bool Event::isConsumed() const { return consumed_; }
} // namespace soil::event
