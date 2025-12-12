
#ifndef SOIL_STAGE_SCENE_SCENE_H
#define SOIL_STAGE_SCENE_SCENE_H
#include "node.h"
#include "video/render/algorithm.hpp"

namespace soil::stage {
class Stage;
}

namespace soil::stage::scene {
namespace component::render {
class DrawableContainerComponent;
}
namespace component {
class UpdateGraphComponent;
}
class Scene : public Node {
 public:
  friend class soil::stage::Stage;
  Scene();
  explicit Scene(const video::render::Algorythm* algorythm);

  ~Scene() override;
  [[nodiscard]] class Stage* Stage() const override;

  virtual void Render(video::render::State& state);
  void Update() override;

 protected:
  virtual void SetStage(class Stage* stage);

  class Stage* stage_;
  const video::render::Algorythm* renderAlgorythm_;
  component::render::DrawableContainerComponent* drawableContainer_;
  component::UpdateGraphComponent* updateGraphComponent_;
};
}  // namespace soil::stage::scene

#endif
