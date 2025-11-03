#ifndef SOIL_STAGE_SCENE_COMPONENT_EVENT_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_EVENT_COMPONENT_H
#include "input/event.h"
#include "stage/event/node.h"
#include "stage/hook/event_hook.hpp"
#include "stage/hook/trigger_hook.h"
#include "stage/scene/component/component.h"
#include "window_event.h"
namespace soil::stage::scene::component {
class EventComponent : public Component,
                       public hook::EventHook<event::Node>,
                       public hook::EventHook<input::Event>,
                       public hook::EventHook<WindowEvent>,
                       public hook::TriggerHook {
 public:
  enum class EventType {
    Node = 0,
    Window,
    Input,
  };
  explicit EventComponent(
      const std::vector<EventType> &events = {},
      const std::vector<soil::stage::hook::TriggerHook::TriggerType> &triggers =
          {});
  ~EventComponent() override = default;
  void ActivateEvents(const std::vector<EventType> &events);
  void ActivateTrigger(
      const std::vector<soil::stage::hook::TriggerHook::TriggerType> &triggers);

  void OnEvent(const event::Node &event) override;
  void OnEvent(const input::Event &event) override;
  void OnEvent(const WindowEvent &event) override;
  void OnTrigger(soil::stage::hook::TriggerHook::TriggerType trigger) override;

 protected:
  void OnStageChanged(soil::stage::Stage *stage,
                      soil::stage::Stage *prevStage) override;

 private:
  soil::stage::Stage *stage_;
  std::vector<EventType> activeEvents_;
  bool activeTrigger_;
};
}  // namespace soil::stage::scene::component
#endif
