#ifndef SOIL_STAGE_SCENE_COMPONENT_INPUT_INPUT_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_INPUT_INPUT_COMPONENT_H

#include "input/event.h"
#include "input/event_map.h"
#include "stage/hook/event_hook.hpp"
#include "stage/scene/component/component.h"
namespace soil::stage::scene::component::input {
class InputComponent : public Component,
                       public hook::EventHook<soil::input::Event> {
 public:
  InputComponent();
  ~InputComponent() override;
  void OnEvent(const soil::input::Event &event) override;
  [[nodiscard]] soil::input::EventMap &EventMap();

 protected:
  void OnStageChanged(soil::stage::Stage *stage,
                      soil::stage::Stage *prevStage) override;

 private:
  Stage *stage_;
  soil::input::EventMap eventMap_;
};
}  // namespace soil::stage::scene::component::input
#endif
