
#ifndef SOIL_STAGE_STAGE_H
#define SOIL_STAGE_STAGE_H

#include <typeindex>

#include "event/engine_event.h"
#include "event/game_event.hpp"
#include "event/stage_event.h"
#include "input/event.h"
#include "interfaces.h"
#include "resources.h"
#include "stage/event/node.h"
#include "stage/hook/event_hook.hpp"
#include "stage/hook/trigger_hook.h"
#include "video/event/window_event.h"

namespace soil::stage {
namespace scene {
class Scene;
}  // namespace scene

class Manager;

class StageNotRegisteredException : public std::runtime_error {
 public:
  StageNotRegisteredException()
      : runtime_error("Stage not registered to stage manager") {}
};

class Stage : public input::EventHandler,
              public video::event::WindowEventHandler,
              public event::StageEventHandler,
              public event::NodeEventHandler,
              public event::GameEventHandler,
              public soil::event::EngineEventHandler {
 public:
  friend class Manager;

  explicit Stage();

  ~Stage() override;

  virtual void Update();

  virtual void Render(video::render::State& state);

  template <class T>
  T AddScene(T scene) {
    using type = std::remove_pointer_t<T>;
    static_assert(std::is_base_of_v<scene::Scene, type>,
                  "feature must be derived from ComponentFeature");
    _addScene(scene);
    return scene;
  }

  void RemoveScene(const scene::Scene* scene);

  [[nodiscard]] Resources& GetResources() const;

  virtual void Load();

  virtual void Unload();

  [[nodiscard]] bool IsLoaded() const;

  void Handle(const input::Event& event) override;

  void Handle(const video::event::WindowEvent& event) override;

  void Handle(const event::StageEvent& event) override;
  void Handle(const event::Node& event) override;

  void Handle(const event::GameEvent& event) override;
  void Handle(const soil::event::EngineEvent& event) override;

  void SetCurrent();

  IManager* Manager() const;

  void AddEventHook(scene::Node* root,
                    soil::stage::hook::EventHook<event::Node>* hook);

  void AddEventHook(soil::stage::hook::EventHook<input::Event>* hook);
  void AddEventHook(
      soil::stage::hook::EventHook<video::event::WindowEvent>* hook);

  void AddEventHook(soil::stage::hook::EventHook<event::GameEvent>* hook);

  void AddEventHook(
      soil::stage::hook::EventHook<soil::event::EngineEvent>* hook);

  void RemoveEventHook(scene::Node* root,
                       soil::stage::hook::EventHook<event::Node>* hook);
  void RemoveEventHook(soil::stage::hook::EventHook<input::Event>* hook);
  void RemoveEventHook(
      soil::stage::hook::EventHook<video::event::WindowEvent>* hook);
  void RemoveEventHook(soil::stage::hook::EventHook<event::GameEvent>* hook);
  void RemoveEventHook(
      soil::stage::hook::EventHook<soil::event::EngineEvent>* hook);

  void AddTriggerHook(soil::stage::hook::TriggerHook* trigger,
                      const soil::stage::hook::TriggerHook::TriggerPoint& at);
  void RemoveTriggerHook(
      soil::stage::hook::TriggerHook* hook,
      const soil::stage::hook::TriggerHook::TriggerPoint& at);

 protected:
  void triggerHooks(const soil::stage::hook::TriggerHook::TriggerPoint& at);

  template <class T>
  void _removeEventHook(
      scene::Node* root, soil::stage::hook::EventHook<T>* eventHook,
      std::unordered_map<scene::Node*,
                         std::vector<soil::stage::hook::EventHook<T>*>>& map) {
    auto itr = map.find(root);
    if (itr == map.end()) {
      return;
    }
    for (auto vItr = itr->second.begin(); vItr != itr->second.end(); ++vItr) {
      if (eventHook == *vItr) {
        itr->second.erase(vItr);
        if (itr->second.empty()) {
          map.erase(itr);
        }
        return;
      }
    }
  }
  void _addScene(scene::Scene* scene);

  [[nodiscard]] virtual std::vector<scene::Scene*> GetScenes() const;

  std::unordered_map<scene::Node*,
                     std::vector<soil::stage::hook::EventHook<event::Node>*>>
      nodeEventHooks_;

  std::vector<soil::stage::hook::EventHook<input::Event>*> inputEventHooks_;

  std::vector<soil::stage::hook::EventHook<video::event::WindowEvent>*>
      windowEventHooks_;

  std::vector<soil::stage::hook::EventHook<event::GameEvent>*> gameEventHooks_;

  std::vector<soil::stage::hook::EventHook<soil::event::EngineEvent>*>
      engineEventHooks_;

  std::unordered_map<soil::stage::hook::TriggerHook::TriggerPoint,
                     std::vector<soil::stage::hook::TriggerHook*>,
                     soil::stage::hook::TriggerHook::TriggerPointEquality>
      triggerHooks_;
  std::vector<soil::stage::hook::TriggerHook*> invalidatedHooks_;
  bool triggering_;

 private:
  IManager* manager_;
  bool loaded_;
  std::vector<scene::Scene*> scenes_;
  Resources* resources_;
};
}  // namespace soil::stage

#endif
