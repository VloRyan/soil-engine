#ifndef SOUND_STAGE_H
#define SOUND_STAGE_H

#include "common/stage.h"
#include "stage/scene/component/sound.h"

namespace soil_samples::sound {

class Stage final : public common::Stage {
 public:
  explicit Stage();
  ~Stage() override = default;

  void OnLoad(soil::stage::scene::Scene* scene) override;
  void Handle(const soil::stage::event::StageEvent& event) override;

 protected:
  void RegisterInputEvents(soil::input::EventMap& eventMap) override;

 private:
  soil::stage::scene::component::Sound* music_;
};
}  // namespace soil_samples::sound

#endif
