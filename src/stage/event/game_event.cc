#include "stage/event/game_event.h"

namespace soil::stage::event {
GameEvent::GameEvent(const int trigger)
    : Event(Type::Game), trigger_(trigger) {}

int GameEvent::Trigger() const { return trigger_; }

bool GameEvent::IsTrigger(const int trigger) const {
  return trigger == trigger_;
}
}  // namespace soil::stage::event
