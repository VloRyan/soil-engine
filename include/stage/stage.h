
#ifndef SOIL_STAGE_STAGE_H
#define SOIL_STAGE_STAGE_H

#include <typeindex>

#include "event/game_event.h"
#include "event/stage_event.h"
#include "input/event.h"
#include "interfaces.h"
#include "resources.h"
#include "stage/event/node.h"
#include "stage/hook/event_hook.hpp"
#include "stage/hook/trigger_hook.h"
#include "window_event.h"

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
              public WindowEventHandler,
              public event::StageEventHandler,
              public soil::event::Observable<event::GameEvent>,
              public event::NodeEventHandler {
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

  void Handle(const WindowEvent& event) override;

  void Handle(const event::StageEvent& event) override;
  void Handle(const event::Node& event) override;

  void SetCurrent();

  IManager* Manager() const;

  void AddEventHook(soil::stage::hook::EventHook<event::Node>* hook) {
    nodeEventHooks_.push_back(hook);
  }
  void AddEventHook(soil::stage::hook::EventHook<input::Event>* hook) {
    inputEventHooks_.push_back(hook);
  }
  void AddEventHook(soil::stage::hook::EventHook<WindowEvent>* hook) {
    windowEventHooks_.push_back(hook);
  }

  /*template <class T>
  void AddEventHook(soil::stage::hook::EventHook<T>* hook) {
    if (typeid(T) == typeid(event::Node)) {
      nodeEventHooks_.push_back(
          static_cast<soil::stage::hook::EventHook<event::Node>*>(hook));
    } else if (typeid(T) == typeid(input::Event)) {
      inputEventHooks_.push_back(
          static_cast<soil::stage::hook::EventHook<input::Event>*>(hook));
    } else if (typeid(T) == typeid(WindowEvent)) {
      windowEventHooks_.push_back(
          static_cast<soil::stage::hook::EventHook<WindowEvent>*>(hook));
    } else {
      throw std::runtime_error("unknown event type");
    }
  }*/

  void RemoveEventHook(soil::stage::hook::EventHook<event::Node>* hook) {
    _removeEventHook(hook, nodeEventHooks_);
  }
  void RemoveEventHook(soil::stage::hook::EventHook<input::Event>* hook) {
    _removeEventHook(hook, inputEventHooks_);
  }
  void RemoveEventHook(soil::stage::hook::EventHook<WindowEvent>* hook) {
    _removeEventHook(hook, windowEventHooks_);
  }
  /*
    template <class T>
    void RemoveEventHook(soil::stage::hook::EventHook<T>* hook) {
      if (typeid(T) == typeid(event::Node)) {
        _removeEventHook(hook, nodeEventHooks_);
      } else if (typeid(T) == typeid(input::Event)) {
        _removeEventHook(hook, inputEventHooks_);
      } else if (typeid(T) == typeid(WindowEvent)) {
        _removeEventHook(hook, windowEventHooks_);
      } else {
        throw std::runtime_error("unknown event type");
      }
    }
  */
  void AddTriggerHook(soil::stage::hook::TriggerHook* trigger);
  void RemoveTriggerHook(soil::stage::hook::TriggerHook* trigger);

 protected:
  void triggerHooks(soil::stage::hook::TriggerHook::TriggerType type);

  template <class T>
  void _removeEventHook(soil::stage::hook::EventHook<T>* eventHook,
                        std::vector<soil::stage::hook::EventHook<T>*> vector) {
    for (auto itr = vector.begin(); itr != vector.end(); ++itr) {
      if (eventHook == *itr) {
        vector.erase(itr);
        return;
      }
    }
  }
  void _addScene(scene::Scene* scene);

  [[nodiscard]] virtual std::vector<scene::Scene*> GetScenes() const;

 private:
  IManager* manager_;
  std::vector<soil::stage::hook::EventHook<event::Node>*> nodeEventHooks_;
  std::vector<soil::stage::hook::EventHook<input::Event>*> inputEventHooks_;
  std::vector<soil::stage::hook::EventHook<WindowEvent>*> windowEventHooks_;

  std::unordered_map<soil::stage::hook::TriggerHook::TriggerType,
                     std::vector<soil::stage::hook::TriggerHook*>>
      triggerHooks_;

  /*std::unordered_map<
      std::type_index,
      std::vector<soil::stage::scene::hook::EventHook<soil::event::Event>*>>
      eventHooks_;*/

  bool loaded_;
  std::vector<scene::Scene*> scenes_;
  Resources* resources_;
};
}  // namespace soil::stage

#endif
