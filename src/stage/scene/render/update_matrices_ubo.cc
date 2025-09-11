#include "stage/scene/render/update_matrices_ubo.h"

namespace soil::stage::scene::render {

    UpdateMatricesUbo::UpdateMatricesUbo(viewer::Node* viewer, const int uboMatricesBindingTarget) :
        RenderHook(HandlerType::None), viewer_(viewer), uboMatricesBindingTarget_(uboMatricesBindingTarget) {}

    void UpdateMatricesUbo::OnRender(video::render::State& state) {
        state.WriteUbo(uboMatricesBindingTarget_, [this](video::buffer::Cursor* cursor) {
            const auto proj = viewer_->GetProjectionMatrix();
            const auto view = viewer_->GetViewMatrix();
            const auto pvMatrix = proj * view;
            cursor->Write(proj);
            cursor->Write(view);
            cursor->Write(glm::inverse(view));
            cursor->Write(pvMatrix);
        });
    }
} // namespace soil::stage::scene::render
