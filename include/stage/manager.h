#ifndef SOIL_STAGE_MANAGER_H_
#define SOIL_STAGE_MANAGER_H_
#include "event/engine_event.h"
#include "input/event.h"
#include "input/manager.h"
#include "resources.h"
#include "stage/interfaces.h"

namespace soil::stage {
class Stage;

class Manager final : public input::EventHandler,
                      public video::event::WindowEventHandler,
                      public soil::event::EngineEventHandler,
                      public IManager {
 public:
  explicit Manager(Resources& resources);
  ~Manager() override;
  void HookTo(
      soil::event::Observable<soil::event::EngineEvent>& engineEventObservable,
      soil::event::Observable<input::Event>& inputEventObservable,
      soil::event::Observable<video::event::WindowEvent>&
          windowEventObservable);

  void RegisterStage(const std::string& name, Stage* stage);
  void SetCurrent(const std::string& name) override;
  void SetCurrent(Stage* stage) override;
  Stage* GetCurrent() const;
  Stage* GetStage(const std::string& name) const override;
  Stage* RemoveStage(const std::string& name) override;
  void Update();
  void Render(video::render::State& state) const;
  void Handle(const input::Event& event) override;
  void Handle(const video::event::WindowEvent& event) override;
  void Handle(const soil::event::EngineEvent& event) override;
  [[nodiscard]] Resources& GetResources() const;

 private:
  std::unordered_map<std::string, Stage*> stages_;
  std::string currentStageName_;
  Stage* currentStage_;
  Stage* nextStage_;
  std::function<const void()> deregisterFun;
  Resources& resources_;
};
}  // namespace soil::stage

#endif
