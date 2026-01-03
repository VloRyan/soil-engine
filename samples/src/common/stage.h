#ifndef SOIL_EXAMPLES_COMMON_STAGE_H
#define SOIL_EXAMPLES_COMMON_STAGE_H
#include "engine.h"
#include "input/event_map.h"
#include "stage/scene/viewer/node.h"
#include "stage/stage.h"

namespace soil_samples::common {
class Stage : public soil::stage::Stage {
 public:
  explicit Stage();
  ~Stage() override = default;

  virtual void OnLoad(soil::stage::scene::Scene* scene) = 0;

  void Load() override;
  void SetBackAction(std::function<void()> backAction);

  void Handle(const soil::event::EngineEvent& event) override;

 protected:
  virtual void RegisterInputEvents(soil::input::EventMap& eventMap) = 0;
  virtual void OnStatsChanges(const soil::Engine::Statistics& stats);
  bool printStatistics_;
  virtual soil::stage::scene::viewer::Node* NewViewer(glm::ivec2 windowSize);
  soil::stage::scene::viewer::Node* viewer_;

 private:
  std::function<void()> backAction_;
};
}  // namespace soil_samples::common

#endif
