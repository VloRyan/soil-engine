#include "event/engine_event.h"
namespace soil::event {
EngineEvent::EngineEvent(class soil::Engine *engine,
                         EngineEvent::CauseType cause)
    : Event(event::Event::Type_t::Engine), Engine(engine), Cause(cause) {}
}  // namespace soil::event
