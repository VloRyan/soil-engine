#ifndef WORLD_STAGE_H
#define WORLD_STAGE_H
#include "basic/shader.h"
#include "basic/shape.h"
#include "common/stage.h"
#include "engine.h"
#include "stage/scene/world/world_node.h"
#include "stage/stage.h"

namespace soil_samples::world {
class Stage final : public common::Stage {
 public:
  explicit Stage();
  ~Stage() override = default;
  void Handle(const soil::WindowEvent& event) override;
  void OnLoad() override;
  void Update() override;

 protected:
  void RegisterInputEvents(soil::input::EventMap& eventMap) override;

 private:
  void initBackground(soil::stage::scene::Scene* scene, byte textureUnit) const;
  void initCarrots(soil::stage::scene::Scene* scene, byte textureUnit);
  std::array<basic::Shape*, 4> shapes_;
  bool printStatistics_;
  soil::stage::scene::world::WorldNode* world_;
  soil::stage::scene::component::CollisionObjectComponent* colObj_;
};
}  // namespace soil_samples::world

#endif
