#ifndef SOIL_ENGINE_SOUND_EVENT_SOURCE_EVENT_H
#define SOIL_ENGINE_SOUND_EVENT_SOURCE_EVENT_H
#include "event.hpp"

namespace soil::sound {
class Source;

namespace event {
class SourceEvent : public Event {
 public:
  enum class TriggerType : std::uint8_t {
    Added,
    PlayStateChanged,
    Removed,
  };

  explicit SourceEvent(TriggerType trigger, sound::Source* source);

  ~SourceEvent() override = default;

  [[nodiscard]] virtual sound::Source* Source() const;

  [[nodiscard]] TriggerType Trigger() const;

 private:
  sound::Source* source_;
  TriggerType trigger_;
};
}  // namespace event
}  // namespace soil::sound
#endif
