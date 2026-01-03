#ifndef WORLD_STAGE_H
#define WORLD_STAGE_H
#include "basic/shape.h"
#include "common/stage.h"
#include "engine.h"
#include "stage/scene/component/world/world_component.h"
#include "stage/stage.h"

namespace soil_samples::world {
class Stage final : public common::Stage {
 public:
  explicit Stage();
  ~Stage() override = default;
  void OnLoad(soil::stage::scene::Scene* scene) override;
  void Update() override;

 protected:
  void RegisterInputEvents(soil::input::EventMap& eventMap) override;
  soil::stage::scene::viewer::Node* NewViewer(glm::ivec2 windowSize) override;

 private:
  void initBackground(soil::stage::scene::Scene* scene, byte textureUnit) const;
  void initCarrots(soil::stage::scene::Scene* scene, byte textureUnit);
  std::array<basic::Shape*, 4> shapes_;
  soil::stage::scene::component::world::WorldComponent* world_;
  soil::stage::scene::component::world::CollisionObjectComponent* colObj_;
};
}  // namespace soil_samples::world

#endif
