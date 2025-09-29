#include "sound/event/source_event.h"

namespace soil::sound::event {
    SourceEvent::SourceEvent(const TriggerType trigger, sound::Source* source) :
        Event(Cause::Source), source_(source), trigger_(trigger) {
    }

    sound::Source* SourceEvent::Source() const {
        return source_;
    }

    SourceEvent::TriggerType SourceEvent::Trigger() const {
        return trigger_;
    }
}
