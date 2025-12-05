#ifndef SOIL_ENGINE_EVENT_H
#define SOIL_ENGINE_EVENT_H
#include "event.hpp"
#include "handler.hpp"
namespace soil {
class Engine;
}
namespace soil::event {

class EngineEvent : public Event {
 public:
  enum class CauseType {
    StatisticsChanged,
  };
  EngineEvent(class soil::Engine* engine, CauseType cause);
  ~EngineEvent() = default;
  CauseType Cause;
  class soil::Engine* Engine;
};
using EngineEventHandler = soil::event::Handler<EngineEvent>;
}  // namespace soil::event
#endif
