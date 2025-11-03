#ifndef SOIL_STAGE_SCENE_RENDER_UPDATE_UBO_H
#define SOIL_STAGE_SCENE_RENDER_UPDATE_UBO_H

#include "stage/hook/trigger_hook.h"
#include "stage/scene/viewer/node.h"
#include "video/render/state.h"

namespace soil::stage::scene::render {
class UpdateMatricesUbo : public hook::TriggerHook {
 public:
  UpdateMatricesUbo(viewer::Node* viewer, int uboMatricesBindingTarget,
                    video::render::State* renderState);

  ~UpdateMatricesUbo() override = default;

  void OnTrigger(soil::stage::hook::TriggerHook::TriggerType trigger) override;

 private:
  int uboMatricesBindingTarget_;
  viewer::Node* viewer_;
  video::render::State* renderState_;
};
}  // namespace soil::stage::scene::render

#endif
