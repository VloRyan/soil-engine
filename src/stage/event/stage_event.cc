#include "stage/event/stage_event.h"

namespace soil::stage::event {
StageEvent::StageEvent(class Stage* stage, StageEvent::TriggerType trigger)
    : Event(Type_t::Stage), Trigger(trigger), Stage(stage) {}

StageEvent StageEvent::MakeActiveStageChanged(class Stage* current,
                                              class Stage* previous) {
  auto event = StageEvent(current, TriggerType::ActiveStageChanged);
  event.PrevStage = previous;
  return event;
}

}  // namespace soil::stage::event
