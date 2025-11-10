#ifndef SOIL_STAGE_SCENE_COMPONENT_EVENT_HOOK_H
#define SOIL_STAGE_SCENE_COMPONENT_EVENT_HOOK_H

#include "input/event.h"
#include "stage/event/node.h"
#include "stage/hook/event_hook.hpp"
#include "stage/hook/trigger_hook.h"
#include "stage/scene/component/component.h"
#include "window_event.h"

namespace soil::stage::scene {
class Scene;
}

namespace soil::stage::scene::component::event {
class Hook : public hook::EventHook<stage::event::Node>,
             public hook::EventHook<input::Event>,
             public hook::EventHook<WindowEvent>,
             public hook::TriggerHook {
 public:
  enum class EventType {
    Node = 0,
    Window,
    Input,
  };
  explicit Hook(const std::vector<EventType> &events = {},
                const std::vector<soil::stage::hook::TriggerHook::TriggerPoint>
                    &points = {});

  ~Hook() override = default;
  void ActivateEvents(const std::vector<EventType> &events);
  void ActivateTrigger(const TriggerPoint &point);
  void RemoveTrigger(const TriggerPoint &point);

  void OnEvent(const stage::event::Node &event) override;
  void OnEvent(const input::Event &event) override;
  void OnEvent(const WindowEvent &event) override;
  void OnTrigger(const TriggerPoint &point) override;

  void SetStage(Stage *stage);

  void SetNodeEventCallback(
      const std::function<void(const stage::event::Node &)> &nodeEventCallback);
  void SetInputEventCallback(
      const std::function<void(const input::Event &)> &inputEventCallback);
  void SetWindowEventCallback(
      const std::function<void(const WindowEvent &)> &windowEventCallback);
  void SetTriggerCallback(
      const std::function<void(const TriggerPoint &point)> &triggerCallback);
  void SetTriggerRoot(Node *triggerRoot);

 protected:
  soil::stage::Stage *stage_;
  std::vector<EventType> activeEvents_;
  std::vector<soil::stage::hook::TriggerHook::TriggerPoint>
      activeTriggerPoints_;
  std::function<void(const stage::event::Node &event)> nodeEventCallback_;
  std::function<void(const input::Event &event)> inputEventCallback_;
  std::function<void(const WindowEvent &event)> windowEventCallback_;
  std::function<void(const TriggerPoint &point)> triggerCallback_;
  stage::scene::Node *triggerRoot_;
};
}  // namespace soil::stage::scene::component::event
#endif
