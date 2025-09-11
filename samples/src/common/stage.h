#ifndef SOIL_ENGINE_STAGE_H
#define SOIL_ENGINE_STAGE_H
#include "input/event_map.h"
#include "stage/scene/input.h"
#include "stage/stage.h"

namespace soil_samples::common {
    class Stage : public soil::stage::Stage {
    public:
        explicit Stage();
        ~Stage() override = default;

        virtual void OnLoad() = 0;

        void Load() override;
        void SetBackAction(std::function<void()> backAction);

    protected:
        virtual void RegisterInputEvents(soil::input::EventMap& eventMap) = 0;

    private:
        std::function<void()> backAction_;
    };
} // namespace soil_samples::common

#endif
