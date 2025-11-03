#ifndef SOIL_STAGE_SOIL_ENGINE_STAGE_H
#define SOIL_STAGE_SOIL_ENGINE_STAGE_H
#include <bitset>
#include <vector>

#include "event/event.hpp"
#include "event/handler.hpp"

namespace soil::stage {
class Stage;
}

namespace soil::stage::event {
struct StageEvent : soil::event::Event {
  enum class TriggerType : std::uint8_t {
    ActiveStageChanged = 0,
    StageChanged,
  };
  explicit StageEvent(class Stage* stage = nullptr,
                      TriggerType trigger = TriggerType::ActiveStageChanged);
  ~StageEvent() override = default;

  static StageEvent MakeActiveStageChanged(Stage* current, Stage* previous);
  ;

  TriggerType Trigger{TriggerType::StageChanged};
  class Stage* Stage{nullptr};
};

using StageEventHandler = soil::event::Handler<StageEvent>;
}  // namespace soil::stage::event
#endif
