
#ifndef SOIL_STAGE_STAGE_H
#define SOIL_STAGE_STAGE_H

#include "event/game_event.h"
#include "event/stage_event.h"
#include "input/event.h"
#include "interfaces.h"
#include "resources.h"
#include "window_event.h"

namespace soil::stage {
namespace scene {
class Scene;
}

class Manager;

class StageNotRegisteredException : public std::runtime_error {
 public:
  StageNotRegisteredException()
      : runtime_error("Stage not registered to stage manager") {}
};

class Stage : public input::EventHandler,
              public WindowEventHandler,
              public event::StageEventHandler,
              public soil::event::Observable<event::GameEvent> {
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
    addScene(scene);
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

  void SetCurrent();

  IManager* Manager() const;

 protected:
  void addScene(scene::Scene* scene);

  [[nodiscard]] virtual std::vector<scene::Scene*> GetScenes() const;

 private:
  IManager* manager_;
  bool loaded_;
  std::vector<scene::Scene*> scenes_;
  Resources* resources_;
};
}  // namespace soil::stage

#endif
