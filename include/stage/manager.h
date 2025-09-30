#ifndef SOIL_STAGE_MANAGER_H_
#define SOIL_STAGE_MANAGER_H_

#include "input/event.h"
#include "input/manager.h"
#include "resources.h"
#include "stage/interfaces.h"

namespace soil::stage {
class Stage;

class Manager final : public input::EventHandler,
                      public WindowEventHandler,
                      public IManager {
 public:
  explicit Manager(Resources* resources);

  ~Manager() override;

  void RegisterStage(const std::string& name, Stage* stage);

  void SetCurrent(const std::string& name) override;

  void SetCurrent(Stage* stage) override;

  Stage* GetCurrent() const;

  Stage* GetStage(const std::string& name) const override;

  Stage* RemoveStage(const std::string& name) override;

  void Update();

  void Render(video::render::State& state) const;

  void Handle(const input::Event& event) override;

  void Handle(const WindowEvent& event) override;

  [[nodiscard]] Resources& GetResources() const;

 private:
  std::unordered_map<std::string, Stage*> stages_;
  std::string currentStageName_;
  Stage* currentStage_;
  Stage* nextStage_;
  std::function<const void()> deregisterFun;
  Resources* resources_;
};
}  // namespace soil::stage

#endif
