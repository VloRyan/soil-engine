#ifndef SOIL_ENGINE_SOUND_EVENT_EVENT_H
#define SOIL_ENGINE_SOUND_EVENT_EVENT_H
#include "event/event.hpp"
#include "event/handler.hpp"

namespace soil::sound::event {
enum class Cause : std::uint8_t {
  Source,
};

struct Event : public soil::event::Event {
  explicit Event(event::Cause cause = Cause::Source)
      : soil::event::Event(Type_t::Sound), Cause(cause) {};
  ~Event() override = default;
  event::Cause Cause{Cause::Source};
};

using EventHandler = soil::event::Handler<Event>;
}  // namespace soil::sound::event

#endif
