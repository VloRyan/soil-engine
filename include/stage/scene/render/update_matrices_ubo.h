#ifndef SOIL_STAGE_SCENE_RENDER_UPDATE_UBO_H
#define SOIL_STAGE_SCENE_RENDER_UPDATE_UBO_H
#include "stage/scene/hook/render_hook.h"
#include "stage/scene/viewer/node.h"

namespace soil::stage::scene::render {
class UpdateMatricesUbo : public hook::RenderHook {
 public:
  UpdateMatricesUbo(viewer::Node* viewer, int uboMatricesBindingTarget);

  ~UpdateMatricesUbo() override = default;

  void OnRender(video::render::State& state) override;

 private:
  int uboMatricesBindingTarget_;
  viewer::Node* viewer_;
};
}  // namespace soil::stage::scene::render

#endif
