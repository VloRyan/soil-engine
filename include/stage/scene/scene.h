
#ifndef SOIL_STAGE_SCENE_SCENE_H
#define SOIL_STAGE_SCENE_SCENE_H
#include "node.h"
#include "video/render/container.h"
#include "video/render/pipeline.h"

namespace soil::stage {
class Stage;
}

namespace soil::stage::scene {
class Scene : public Node {
 public:
  friend class soil::stage::Stage;

  explicit Scene();

  ~Scene() override;

  void Update() override;

  virtual void Render(video::render::State& state);

  [[nodiscard]] Stage* GetStage() const;

  [[nodiscard]] virtual video::render::Pipeline* GetPipeline() const;

  virtual void SetPipeline(video::render::Pipeline* pipeline);

  [[nodiscard]] virtual video::render::Container* GetRenderContainer() const;

 protected:
  virtual void SetStage(Stage* stage);

 private:
  Stage* stage_;
  std::vector<Node*> nodesToDelete_;
  video::render::Container* renderContainer_;
  video::render::Pipeline* pipeline_;
};
}  // namespace soil::stage::scene

#endif
