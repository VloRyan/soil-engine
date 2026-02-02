#ifndef SOIL_EXAMPLES_TEXT_STAGE_H
#define SOIL_EXAMPLES_TEXT_STAGE_H
#include "common/stage.h"
#include "file/sprite_sheet.h"
#include "node.h"
#include "stage/stage.h"
#include "common/node/label.h"
#include "common/node/pane.h"

namespace soil_samples::text {
class Stage final : public common::Stage {
 public:
  explicit Stage();
  ~Stage() override = default;
  void Update() override;
  void OnLoad(soil::stage::scene::Scene* scene) override;

 protected:
  void RegisterInputEvents(soil::input::EventMap& eventMap) override;
  void OnStatsChanges(const soil::Engine::Statistics& stats) override;

 private:
  void initBackground(soil::stage::scene::Node* parent);

  common::node::Label* text_;
  common::node::Label* description_;
  common::node::Label* bouncingText_;

  common::node::Pane* bgPane_;
  //basic::Shape* bgShape_;
  glm::vec2 bounceTextVelocity_;
  float bounceTextGlowVelocity_;
  int fastChangeIndex_;
  soil::file::SpriteSheet spriteSheet_;
  bool statisticsAsText_;
};
}  // namespace soil_samples::text

#endif
