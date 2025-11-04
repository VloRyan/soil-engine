#ifndef TEST_STAGE_SCENE_HOOK_MOCKS
#define TEST_STAGE_SCENE_HOOK_MOCKS

#include "stage/hook/event_hook.hpp"
#include "stage/hook/trigger_hook.h"
#include "stage/scene/scene.h"

namespace soil::stage::hook {

class HookMock : public TriggerHook, public EventHook<event::Node> {
 public:
  explicit HookMock(const std::vector<TriggerType>& triggers)
      : TriggerHook(triggers), Calls() {}

  std::vector<event::Node> EventsReceived;
  struct {
    int Perform = 0;
  } Calls;

  void Reset() {
    EventsReceived.clear();
    Calls.Perform = 0;
  }

  void OnEvent(const event::Node& event) override {
    EventsReceived.push_back(event);
  }

  void OnTrigger(TriggerType trigger) override { Calls.Perform++; }
};

}  // namespace soil::stage::hook
#endif
