#include "stage/scene/component/event/event_component.h"

#include "stage/scene/node.h"
#include "stage/stage.h"
namespace soil::stage::scene::component::event {
EventComponent::EventComponent(const Type type,
                               const EventComponentDefinition &definition)
    : Component(type),
      Hook(definition.Events, definition.TriggerPoints),
      globalNodeEvents_(definition.GlobalNodeEvents) {}

void EventComponent::OnStageChanged(soil::stage::Stage *stage,
                                    soil::stage::Stage *prevStage) {
  Component::OnStageChanged(stage, prevStage);
  Hook::SetStage(stage);
}

void EventComponent::SetParent(Node *parent) {
  Component::SetParent(parent);
  if (parent == parent_) {
    return;
  }
  if (parent == nullptr) {
    SetStage(nullptr);
    return;
  }
  auto *root = parent->Root();
  SetStage(parent->Stage());
  if (!globalNodeEvents_ && root != nullptr) {
    SetTriggerRoot(root);
  }
}
}  // namespace soil::stage::scene::component::event
