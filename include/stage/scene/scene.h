
#ifndef SOIL_STAGE_SCENE_SCENE_H
#define SOIL_STAGE_SCENE_SCENE_H
#include "node.h"
#include "video/render/algorithm.hpp"

namespace soil::stage {
class Stage;
}

namespace soil::stage::scene {
class Scene : public Node {
 public:
  friend class soil::stage::Stage;

  explicit Scene();

  ~Scene() override;
  [[nodiscard]] class Stage* Stage() const override;

 protected:
  virtual void SetStage(class Stage* stage);

  class Stage* stage_;
};
}  // namespace soil::stage::scene

#endif
