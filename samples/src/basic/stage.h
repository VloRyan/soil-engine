#ifndef BASIC_STAGE_H
#define BASIC_STAGE_H

#include "common/stage.h"
#include "common/component/shape.h"

namespace soil_samples::basic {
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
  void initBackground(soil::stage::scene::Scene* scene, byte textureUnit) const;
  void initCarrots(soil::stage::scene::Scene* scene, byte textureUnit);
  std::array<common::component::Shape*, SHAPES_PER_DIM * SHAPES_PER_DIM> shapes_;
};
}  // namespace soil_samples::basic

#endif
