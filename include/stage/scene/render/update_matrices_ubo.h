#ifndef SOIL_STAGE_SCENE_RENDER_UPDATE_UBO_H
#define SOIL_STAGE_SCENE_RENDER_UPDATE_UBO_H
#include "stage/scene/hook/hook.h"
#include "stage/scene/viewer/node.h"
#include "video/render/state.h"

namespace soil::stage::scene::render {
class UpdateMatricesUbo : public hook::Hook {
 public:
  UpdateMatricesUbo(viewer::Node* viewer, int uboMatricesBindingTarget,
                    video::render::State* renderState);

  ~UpdateMatricesUbo() override = default;

  void Perform(hook::Hook::Trigger_t trigger) override;

 private:
  int uboMatricesBindingTarget_;
  viewer::Node* viewer_;
  video::render::State* renderState_;
};
}  // namespace soil::stage::scene::render

#endif
