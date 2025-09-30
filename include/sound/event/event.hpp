#ifndef SOIL_ENGINE_SOUND_EVENT_EVENT_H
#define SOIL_ENGINE_SOUND_EVENT_EVENT_H
#include "event/event.h"
#include "event/handler.hpp"

namespace soil::sound::event {
enum class Cause : std::uint8_t {
  Source,
};

class Event : public soil::event::Event {
 public:
  explicit Event(const Cause cause)
      : soil::event::Event(Type::Sound), cause_(cause) {}

  ~Event() override = default;

  event::Cause Cause() const noexcept { return cause_; }

 protected:
  event::Cause cause_;
};

using EventHandler = soil::event::Handler<Event>;
}  // namespace soil::sound::event

#endif
