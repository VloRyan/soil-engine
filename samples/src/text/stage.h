#ifndef TEXT_STAGE_H
#define TEXT_STAGE_H
#include "basic/shape.h"
#include "common/stage.h"
#include "file/sprite_sheet.h"
#include "node.h"
#include "stage/stage.h"

namespace soil_samples::text {
class Stage final : public common::Stage {
 public:
  explicit Stage();
  ~Stage() override = default;
  void Handle(const soil::video::event::WindowEvent& event) override;
  void Update() override;
  void OnLoad(soil::stage::scene::Scene* scene) override;

 protected:
  void RegisterInputEvents(soil::input::EventMap& eventMap) override;
  void OnStatsChanges(const soil::Engine::Statistics& stats) override;

 private:
  void initBackground(soil::stage::scene::Scene* scene, int textureSlot);

  Node* text_;
  soil::stage::scene::Node* bgNode_;
  basic::Shape* bgShape_;
  Node* description_;
  Node* bounceText_;
  glm::vec2 bounceTextVelocity;
  float bounceTextGlowVelocity;
  int fastChangeIndex = -1;
  soil::file::SpriteSheet spriteSheet_;
};
}  // namespace soil_samples::text

#endif
