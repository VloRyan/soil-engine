#include "stage.h"

#include <utility>

#include "stage/scene/component/input/input_component.h"
#include "stage/scene/scene.h"

namespace soil_samples::common {
Stage::Stage() : backAction_(nullptr) {}

void Stage::Load() {
  OnLoad();
  if (GetScenes().size() != 1) {
    throw std::runtime_error("expected one scene after OnLoad()");
  }
  auto* scene = GetScenes()[0];
  auto* inputComp = scene->AddComponent(
      new soil::stage::scene::component::input::InputComponent());
  auto& inputMap = inputComp->EventMap();
  if (backAction_ != nullptr) {
    inputMap.                                                  //
        AddKeyMapping(soil::input::Keys::Escape,               //
                      soil::input::Event::StateType::Release,  //
                      [this](const soil::input::Event&) {
                        backAction_();  //
                      });
  }
  RegisterInputEvents(inputMap);
  soil::stage::Stage::Load();
}

void Stage::SetBackAction(std::function<void()> backAction) {
  backAction_ = std::move(backAction);
}
}  // namespace soil_samples::common
