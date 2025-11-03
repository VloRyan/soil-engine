#ifndef SOIL_ENGINE_SOUND_EVENT_SOURCE_EVENT_H
#define SOIL_ENGINE_SOUND_EVENT_SOURCE_EVENT_H
#include "event.hpp"

namespace soil::sound {
class Source;

namespace event {
struct SourceEvent : public Event {
 public:
  enum class TriggerType : std::uint8_t {
    Added,
    PlayStateChanged,
    Removed,
  };
  explicit SourceEvent(sound::Source* source = nullptr,
                       TriggerType trigger = TriggerType::Added)
      : Event(Cause::Source), Source(source), Trigger(trigger) {}
  ~SourceEvent() override = default;
  sound::Source* Source{nullptr};
  TriggerType Trigger{TriggerType::Added};
};
}  // namespace event
}  // namespace soil::sound
#endif
