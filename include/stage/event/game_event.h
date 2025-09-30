#ifndef SOIL_ENGINE_GAME_EVENT_H
#define SOIL_ENGINE_GAME_EVENT_H
#include "event/event.h"
#include "event/handler.hpp"

namespace soil::stage::event {
class GameEvent : soil::event::Event {
 public:
  explicit GameEvent(int trigger);

  ~GameEvent() override = default;

  [[nodiscard]] int Trigger() const;

  [[nodiscard]] bool IsTrigger(int trigger) const;

 private:
  int trigger_;
};

using GameEventHandler = soil::event::Handler<GameEvent>;
}  // namespace soil::stage::event

#endif
