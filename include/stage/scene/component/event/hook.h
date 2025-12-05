#ifndef SOIL_STAGE_SCENE_COMPONENT_EVENT_HOOK_H
#define SOIL_STAGE_SCENE_COMPONENT_EVENT_HOOK_H

#include "event/engine_event.h"
#include "input/event.h"
#include "stage/event/game_event.hpp"
#include "stage/event/node.h"
#include "stage/hook/event_hook.hpp"
#include "stage/hook/trigger_hook.h"
#include "stage/scene/component/component.h"
#include "video/event/window_event.h"

namespace soil::stage::scene {
class Scene;
}

namespace soil::stage::scene::component::event {
class Hook : public hook::EventHook<stage::event::Node>,
             public hook::EventHook<input::Event>,
             public hook::EventHook<video::event::WindowEvent>,
             public hook::EventHook<stage::event::GameEvent>,
             public hook::EventHook<soil::event::EngineEvent>,
             public hook::TriggerHook {
 public:
  enum class EventType {
    Node = 0,
    Window,
    Input,
    Game,
    Engine,
  };
  explicit Hook(const std::vector<EventType> &events = {},
                const std::vector<soil::stage::hook::TriggerHook::TriggerPoint>
                    &points = {});

  ~Hook() override;
  void ActivateEvents(const std::vector<EventType> &events);
  void ActivateTrigger(const TriggerPoint &point);
  void RemoveTrigger(const TriggerPoint &point);

  void OnEvent(const stage::event::Node &event) override;
  void OnEvent(const input::Event &event) override;
  void OnEvent(const video::event::WindowEvent &event) override;
  void OnEvent(const stage::event::GameEvent &event) override;
  void OnEvent(const soil::event::EngineEvent &event) override;
  void OnTrigger(const TriggerPoint &point) override;

  void SetStage(Stage *stage);

  void SetNodeEventCallback(
      const std::function<void(const stage::event::Node &)> &nodeEventCallback);
  void SetInputEventCallback(
      const std::function<void(const input::Event &)> &inputEventCallback);
  void SetWindowEventCallback(
      const std::function<void(const video::event::WindowEvent &)>
          &windowEventCallback);
  void SetTriggerCallback(
      const std::function<void(const TriggerPoint &point)> &triggerCallback);
  void SetGameEventCallback(
      const std::function<void(const stage::event::GameEvent &)>
          &gameEventCallback);
  void SetEngineEventCallback(
      const std::function<void(const soil::event::EngineEvent &)>
          &engineEventCallback);
  void SetTriggerRoot(Node *triggerRoot);

 protected:
  soil::stage::Stage *stage_;
  std::vector<EventType> activeEvents_;
  std::vector<soil::stage::hook::TriggerHook::TriggerPoint>
      activeTriggerPoints_;
  std::function<void(const stage::event::Node &event)> nodeEventCallback_;
  std::function<void(const input::Event &event)> inputEventCallback_;
  std::function<void(const video::event::WindowEvent &event)>
      windowEventCallback_;
  std::function<void(const soil::event::EngineEvent &event)>
      engineEventCallback_;
  std::function<void(const TriggerPoint &point)> triggerCallback_;
  std::function<void(const stage::event::GameEvent &event)> gameEventCallback_;
  stage::scene::Node *triggerRoot_;
};
}  // namespace soil::stage::scene::component::event
#endif
