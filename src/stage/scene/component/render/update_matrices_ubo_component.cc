#include "stage/scene/component/render/update_matrices_ubo_component.h"

#include "stage/stage.h"
namespace soil::stage::scene::component::render {
UpdateMatricesUboComponent::UpdateMatricesUboComponent(
    const viewer::Node *viewer, int uboMatricesBindingTarget,
    video::render::State *renderState)
    : event::EventComponent(Type::Transform, {}),
      renderState_(renderState),
      viewer_(viewer),
      uboMatricesBindingTarget_(uboMatricesBindingTarget) {}

void UpdateMatricesUboComponent::OnTrigger(
    const hook::TriggerHook::TriggerPoint &point) {
  renderState_->WriteUbo(uboMatricesBindingTarget_,
                         [this](video::buffer::Cursor &cursor) {
                           const auto proj = viewer_->GetProjectionMatrix();
                           const auto view = viewer_->GetViewMatrix();
                           const auto pvMatrix = proj * view;
                           cursor.Write(proj);
                           cursor.Write(view);
                           cursor.Write(glm::inverse(view));
                           cursor.Write(pvMatrix);
                         });
}

void UpdateMatricesUboComponent::SetParent(soil::stage::scene::Node *parent) {
  if (parent_ == parent) {
    return;
  }
  if (parent_ != nullptr) {
    RemoveTrigger({
        .Root = parent_->Root(),
        .TriggerType = TriggerType::BeforeRender,
    });
  }
  if (parent != nullptr) {
    ActivateTrigger({
        .Root = parent->Root(),
        .TriggerType = TriggerType::BeforeRender,
    });
  }
  event::EventComponent::SetParent(parent);
}
}  // namespace soil::stage::scene::component::render
