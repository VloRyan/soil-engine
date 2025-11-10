#ifndef TEST_STAGE_MOCKS
#define TEST_STAGE_MOCKS

#include "stage/stage.h"

namespace soil::stage {
class StageMock : public Stage {
 public:
  int HandleInputEventCalledCount = 0;
  int HandleWindowEventCalledCount = 0;
  int UpdateCalledCount = 0;
  int RenderCalledCount = 0;
  bool CallRealRender = false;

  const std::unordered_map<
      scene::Node*, std::vector<soil::stage::hook::EventHook<event::Node>*>>&
  NodeEventHooks() {
    return nodeEventHooks_;
  };

  const std::vector<soil::stage::hook::EventHook<input::Event>*>&
  InputEventHooks() {
    return inputEventHooks_;
  };

  const std::vector<soil::stage::hook::EventHook<WindowEvent>*>&
  WindowEventHooks() {
    return windowEventHooks_;
  }

  const std::unordered_map<
      soil::stage::hook::TriggerHook::TriggerPoint,
      std::vector<soil::stage::hook::TriggerHook*>,
      soil::stage::hook::TriggerHook::TriggerPointEquality>&
  TriggerHooks() {
    return triggerHooks_;
  }

  void ResetMocks() {
    HandleInputEventCalledCount = 0;
    HandleWindowEventCalledCount = 0;
  }

  void Handle(const input::Event& event) override {
    HandleInputEventCalledCount++;
    Stage::Handle(event);
  }

  void Handle(const WindowEvent& event) override {
    HandleWindowEventCalledCount++;
    Stage::Handle(event);
  }

  void Update() override {
    UpdateCalledCount++;
    Stage::Update();
  }

  void Render(video::render::State& state) override {
    RenderCalledCount++;
    if (CallRealRender) {
      Stage::Render(state);
    }
  }
};
}  // namespace soil::stage
#endif
