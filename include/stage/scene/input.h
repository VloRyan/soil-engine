#ifndef SOIL_STAGE_SCENE_INPUT_H
#define SOIL_STAGE_SCENE_INPUT_H

#include "input/event_map.h"
#include "node.h"
#include "stage/hook/event_hook.hpp"

namespace soil::stage::scene {
class Input final : public Node, public hook::EventHook<input::Event> {
 public:
  explicit Input();

  [[nodiscard]] input::EventMap& GetEventMap();

  void OnEvent(const input::Event& event) override;

 protected:
  void OnStageChanged(soil::stage::Stage* stage,
                      soil::stage::Stage* prevStage) override;

 private:
  input::EventMap eventMap_;
};
}  // namespace soil::stage::scene

#endif
