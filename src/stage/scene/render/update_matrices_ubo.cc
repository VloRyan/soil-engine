#include "stage/scene/render/update_matrices_ubo.h"

#include "stage/scene/scene.h"
#include "stage/stage.h"
namespace soil::stage::scene::render {

UpdateMatricesUbo::UpdateMatricesUbo(viewer::Node* viewer,
                                     const int uboMatricesBindingTarget,
                                     video::render::State* renderState)
    : Hook({Hook::Trigger_t::Render}),
      viewer_(viewer),
      uboMatricesBindingTarget_(uboMatricesBindingTarget),
      renderState_(renderState) {}

void UpdateMatricesUbo::Perform(hook::Hook::Trigger_t trigger) {
  renderState_->WriteUbo(uboMatricesBindingTarget_,
                         [this](video::buffer::Cursor* cursor) {
                           const auto proj = viewer_->GetProjectionMatrix();
                           const auto view = viewer_->GetViewMatrix();
                           const auto pvMatrix = proj * view;
                           cursor->Write(proj);
                           cursor->Write(view);
                           cursor->Write(glm::inverse(view));
                           cursor->Write(pvMatrix);
                         });
}
}  // namespace soil::stage::scene::render
