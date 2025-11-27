#ifndef TEST_STAGE_SCENE_HOOK_MOCKS
#define TEST_STAGE_SCENE_HOOK_MOCKS

#include "stage/hook/event_hook.hpp"
#include "stage/hook/trigger_hook.h"

namespace soil::stage::hook {

class HookMock : public TriggerHook,
                 public EventHook<event::Node>,
                 public EventHook<WindowEvent>,
                 public EventHook<input::Event>,
                 public EventHook<event::GameEvent> {
 public:
  explicit HookMock() : TriggerHook() {}

  std::vector<event::Node> NodeEventsReceived;
  std::vector<WindowEvent> WindowEventsReceived;
  std::vector<input::Event> InputEventsReceived;
  std::vector<TriggerPoint> TriggerReceived;
  std::vector<event::GameEvent> GameEventsReceived;
  std::function<void(const TriggerPoint& point)> OnTriggerFunc{nullptr};

  void Reset() {
    NodeEventsReceived.clear();
    WindowEventsReceived.clear();
    InputEventsReceived.clear();
    GameEventsReceived.clear();
    TriggerReceived.clear();
  }

  void OnEvent(const event::Node& event) override {
    NodeEventsReceived.push_back(event);
  }
  void OnEvent(const WindowEvent& event) override {
    WindowEventsReceived.push_back(event);
  }
  void OnEvent(const input::Event& event) override {
    InputEventsReceived.push_back(event);
  }
  void OnEvent(const event::GameEvent& event) override {
    GameEventsReceived.push_back(event);
  }

  void OnTrigger(const TriggerPoint& point) override {
    if (OnTriggerFunc != nullptr) {
      OnTriggerFunc(point);
    }
    TriggerReceived.push_back(point);
  }
};

}  // namespace soil::stage::hook
#endif
