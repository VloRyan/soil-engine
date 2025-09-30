#include "stage/event/stage_event.h"

namespace soil::stage::event {
    StageEvent::StageEvent(const TriggerType triggerType) :
        Event(Type::Stage), triggerType_(triggerType), stage_(nullptr), previousStage_(nullptr) {
    }

    Stage* StageEvent::GetStage() const {
        return stage_;
    }

    Stage* StageEvent::GetPreviousStage() const {
        return previousStage_;
    }

    bool StageEvent::GetStateChangeFlag(const int flag) const {
        return stateChangeFlags_[flag];
    }

    StageEvent::TriggerType StageEvent::Trigger() const {
        return triggerType_;
    }

    StageEvent StageEvent::MakeActiveStageChanged(Stage* current, Stage* previous) {
        auto event = StageEvent(TriggerType::ActiveStageChanged);
        event.stage_ = current;
        event.previousStage_ = previous;
        return event;
    }

    StageEvent StageEvent::MakeStateChanged(Stage* stage, const std::vector<int>& changeFlags) {
        auto event = StageEvent(TriggerType::StageChanged);
        event.stage_ = stage;
        for (auto i = 0; i < changeFlags.size(); i++) {
            event.stateChangeFlags_[i] = true;
        }
        return event;
    }
}
