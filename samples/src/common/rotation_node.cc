#include "rotation_node.h"

#include <glm/gtc/matrix_transform.hpp>

#include "stage/scene/component/transform_component.h"
#include "stage/stage.h"
namespace soil_samples::common {
RotationNode::RotationNode(const float initialRotation, const float velocity)
    : Node(Type::Transform), velocity_(velocity), ticks(0) {
  auto transform = transform_->GetLocalMatrix();
  transform = glm::rotate(transform, initialRotation, glm::vec3(0.F, 0.F, 1.F));
  transform_->SetLocalMatrix(transform);
  Node::SetUpdateType(UpdateType::Active);
}
void RotationNode::OnTrigger(
    const soil::stage::hook::TriggerHook::TriggerPoint &point) {
  Update();
}
void RotationNode::OnStageChanged(soil::stage::Stage *stage,
                                  soil::stage::Stage *prevStage) {
  Node::OnStageChanged(stage, prevStage);
  if (prevStage != nullptr) {
    prevStage->RemoveTriggerHook(
        this,
        {.TriggerType =
             soil::stage::hook::TriggerHook::TriggerType::BeforeUpdateScene});
  }
  if (stage != nullptr) {
    stage->AddTriggerHook(
        this,
        {.TriggerType =
             soil::stage::hook::TriggerHook::TriggerType::BeforeUpdateScene});
  }
}
void RotationNode::Update() {
  auto transform = transform_->GetLocalMatrix();
  transform = glm::rotate(transform, velocity_, glm::vec3(0.F, 0.F, 1.F));
  transform_->SetLocalMatrix(transform);
  Node::Update();
}
}  // namespace soil_samples::common
