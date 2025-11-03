#ifndef SOIL_ENGINE_GAME_EVENT_H
#define SOIL_ENGINE_GAME_EVENT_H
#include "event/event.hpp"
#include "event/handler.hpp"

namespace soil::stage::event {
struct GameEvent : soil::event::Event {
  ~GameEvent() override = default;
  int Trigger;
};

using GameEventHandler = soil::event::Handler<GameEvent>;
}  // namespace soil::stage::event

#endif
