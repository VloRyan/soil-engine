#ifndef LINE_STAGE_H
#define LINE_STAGE_H
#include <stage/stage.h>

#include "common/stage.h"
#include "line_instance.h"

namespace soil_samples::line {
constexpr int MAX_LINES = 32;

class Stage final : public common::Stage {
 public:
  explicit Stage();
  ~Stage() override = default;

  void OnLoad() override;

 protected:
  void RegisterInputEvents(soil::input::EventMap& eventMap) override;
  void OnStatsChanges(const soil::Engine::Statistics& stats) override;

 private:
  void initLines(soil::stage::scene::Scene* scene);
  std::array<LineInstance*, MAX_LINES> lines_;
  int offset_;
};
}  // namespace soil_samples::line

#endif
