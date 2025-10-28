#ifndef TEST_STAGE_SCENE_HOOK_MOCKS
#define TEST_STAGE_SCENE_HOOK_MOCKS

#include "stage/scene/scene.h"

namespace soil::stage::scene::hook {

class HookMock : public Hook {
 public:
  explicit HookMock(const std::vector<Trigger_t>& triggers,
                    HandlerType handlerType = HandlerType::Component)
      : Hook(triggers, handlerType), Calls() {}

  std::vector<event::Component> EventsReceived;
  struct {
    int Perform = 0;
  } Calls;

  void Reset() {
    EventsReceived.clear();
    Calls.Perform = 0;
  }

  void Handle(const event::Component& event) override {
    EventsReceived.push_back(event);
  }

  void Perform(Trigger_t trigger) override { Calls.Perform++; }
};
}  // namespace soil::stage::scene::hook
#endif
