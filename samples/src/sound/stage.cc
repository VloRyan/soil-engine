#include "stage.h"

#include <asset.h>

#include <string>

#include "stage/scene/component/sound.h"
#include "stage/scene/scene.h"
#include "stage/stage.h"

namespace soil_samples::sound {
Stage::Stage() : music_(nullptr) {}

void Stage::OnLoad() {
  auto* scene = AddScene(new soil::stage::scene::Scene());
  auto* source = GetResources().GetSource(
      asset::GetPath("Sounds/Purple Black Loop Mono.ogg"), true);
  auto* node = scene->AddChild(
      new soil::stage::scene::Node(soil::stage::scene::Node::Type::Sound));
  music_ = node->AddComponent(new soil::stage::scene::component::Sound(source));
  music_->Play();
}

void Stage::Handle(const soil::stage::event::StageEvent& event) {
  if (music_ != nullptr &&
      event.Trigger ==
          soil::stage::event::StageEvent::TriggerType::ActiveStageChanged) {
    if (event.Stage == this) {
      music_->Play();
    } else {
      music_->Stop();
    }
  }
  soil::stage::Stage::Handle(event);
}

void Stage::RegisterInputEvents(soil::input::EventMap& eventMap) {}

}  // namespace soil_samples::sound
