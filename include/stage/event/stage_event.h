#ifndef  SOIL_STAGE_SOIL_ENGINE_STAGE_H
#define  SOIL_STAGE_SOIL_ENGINE_STAGE_H
#include <vector>
#include <bitset>

#include "event/event.h"
#include "event/handler.hpp"

namespace soil::stage {
    class Stage;
}

namespace soil::stage::event {
    class StageEvent : soil::event::Event {
    public:
        enum class TriggerType : std::uint8_t {
            ActiveStageChanged = 0,
            StageChanged,
        };


        ~StageEvent() override = default;

        [[nodiscard]] TriggerType Trigger() const;

        [[nodiscard]] Stage* GetStage() const;

        [[nodiscard]] Stage* GetPreviousStage() const;

        [[nodiscard]] virtual bool GetStateChangeFlag(int flag) const;

        static StageEvent MakeActiveStageChanged(Stage* current, Stage* previous);

        static StageEvent MakeStateChanged(Stage* stage, const std::vector<int>& changeFlags);

    private:
        explicit StageEvent(TriggerType triggerType);

        TriggerType triggerType_;
        Stage* stage_;
        Stage* previousStage_;
        std::bitset<32> stateChangeFlags_;
    };

    using StageEventHandler = soil::event::Handler<StageEvent>;
}
#endif
