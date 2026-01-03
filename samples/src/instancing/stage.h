#ifndef INSTANCING_STAGE_H
#define INSTANCING_STAGE_H
#include <stage/stage.h>

#include "common/stage.h"
#include "shape_instance.h"

namespace soil_samples::instancing {
constexpr int SHAPES_PER_DIM = 8;

class Stage final : public common::Stage {
 public:
  explicit Stage();
  ~Stage() override = default;
  void OnLoad(soil::stage::scene::Scene* scene) override;

 protected:
  void RegisterInputEvents(soil::input::EventMap& eventMap) override;
  soil::stage::scene::viewer::Node* NewViewer(glm::ivec2 windowSize) override;

 private:
  void initBackground(soil::stage::scene::Scene* scene, int textureIndex);
  void initCarrots(soil::stage::scene::Scene* scene, int textureIndex);
  std::array<ShapeInstance*, SHAPES_PER_DIM * SHAPES_PER_DIM> shapes_;
};
}  // namespace soil_samples::instancing

#endif
