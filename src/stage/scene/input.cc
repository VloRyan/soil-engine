
#include "stage/scene/input.h"

#include "stage/stage.h"

namespace soil::stage::scene {
Input::Input() : Node(Type::Input) {}

void Input::OnStageChanged(soil::stage::Stage* stage,
                           soil::stage::Stage* prevStage) {
  Node::OnStageChanged(stage, prevStage);
  if (prevStage != nullptr) {
    prevStage->RemoveEventHook(this);
  }
  if (stage != nullptr) {
    stage->AddEventHook(this);
  }
}

input::EventMap& Input::GetEventMap() { return eventMap_; }

void Input::OnEvent(const input::Event& event) {
  Node::Handle(event);
  eventMap_.Handle(event);
}
}  // namespace soil::stage::scene
