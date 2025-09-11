#ifndef TEST_STAGE_SCENE_HOOK_MOCKS
#define TEST_STAGE_SCENE_HOOK_MOCKS


#include "stage/scene/scene.h"

namespace soil::stage::scene::hook {


    class RenderHookMock : public RenderHook {
    public:
        explicit RenderHookMock(const HandlerType type = HandlerType::Component) : RenderHook(type) {}

        std::vector<event::Component> EventsReceived;
        int OnRenderCalledCount = 0;

        void ResetMocks() {
            EventsReceived.clear();
        }

        void Handle(const event::Component& event) override {
            EventsReceived.push_back(event);
        }

        void OnRender(video::render::State& state) override {
            OnRenderCalledCount++;
        }
    };

    class UpdateHookMock : public UpdateHook {
    public:
        explicit UpdateHookMock(const HandlerType type = HandlerType::Component) : UpdateHook(type) {}

        std::vector<event::Component> EventsReceived;
        int OnUpdateCalledCount = 0;

        void ResetMocks() {
            EventsReceived.clear();
        }

        void Handle(const event::Component& event) override {
            EventsReceived.push_back(event);
        }

        void OnUpdate() override {
            OnUpdateCalledCount++;
        }
    };
} // namespace soil::stage::scene::hook
#endif
