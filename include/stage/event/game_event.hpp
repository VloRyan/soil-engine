#ifndef SOIL_ENGINE_GAME_EVENT_HPP
#define SOIL_ENGINE_GAME_EVENT_HPP
#include "event/event.hpp"
#include "event/handler.hpp"

namespace soil::stage::event {

struct GameEvent : soil::event::Event {
  ~GameEvent() override = default;

 protected:
  GameEvent() : Event(Type_t::Game) {};
};
using GameEventHandler = soil::event::Handler<GameEvent>;
}  // namespace soil::stage::event

#endif
