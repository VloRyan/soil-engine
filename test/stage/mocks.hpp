#ifndef TEST_STAGE_MOCKS
#define TEST_STAGE_MOCKS

#include "stage/stage.h"

namespace soil::stage {
class StageMock : public Stage {
 public:
  ;
  struct Calls_t {
    int HandleNodeEvent{0};
    int HandleInputEvent{0};
    int HandleWindowEvent{0};
    int Update{0};
    int Render{0};
  } Calls;

  bool CallRealRender = false;
  std::vector<event::Node> NodeEvents;
  std::vector<input::Event> InputEvents;
  std::vector<WindowEvent> WindowEvents;

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
    Calls = Calls_t{};
    CallRealRender = false;
    NodeEvents.clear();
    InputEvents.clear();
    WindowEvents.clear();
  }

  void Handle(const event::Node& event) override {
    Calls.HandleNodeEvent++;
    NodeEvents.push_back(event);
    Stage::Handle(event);
  }

  void Handle(const input::Event& event) override {
    Calls.HandleInputEvent++;
    InputEvents.push_back(event);
    Stage::Handle(event);
  }

  void Handle(const WindowEvent& event) override {
    Calls.HandleWindowEvent++;
    WindowEvents.push_back(event);
    Stage::Handle(event);
  }

  void Update() override {
    Calls.Update++;
    Stage::Update();
  }

  void Render(video::render::State& state) override {
    Calls.Render++;
    if (CallRealRender) {
      Stage::Render(state);
    }
  }
};
}  // namespace soil::stage
#endif
