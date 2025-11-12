#include "stage/scene/component/input/input_component.h"

#include "stage/stage.h"
namespace soil::stage::scene::component::input {
InputComponent::InputComponent()
    : Component(Type::Interaction), stage_(nullptr) {}

InputComponent::~InputComponent() {
  if (stage_) {
    stage_->RemoveEventHook(this);
  }
}
void InputComponent::OnEvent(const soil::input::Event &event) {
  eventMap_.Handle(event);
}

void InputComponent::OnStageChanged(soil::stage::Stage *stage,
                                    soil::stage::Stage *prevStage) {
  if (stage_ == stage) {
    return;
  }
  if (stage_) {
    stage_->RemoveEventHook(this);
  }
  stage_ = stage;
  if (stage_) {
    stage_->AddEventHook(this);
  }
}
soil::input::EventMap &InputComponent::EventMap() { return eventMap_; }

}  // namespace soil::stage::scene::component::input
