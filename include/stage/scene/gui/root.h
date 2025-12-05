#ifndef SOIL_STAGE_SCENE_GUI_ROOT_H
#define SOIL_STAGE_SCENE_GUI_ROOT_H

#include "rectangle.h"
#include "stage/hook/event_hook.hpp"

namespace soil::stage::scene::gui {
class Root final : public Rectangle,
                   public hook::EventHook<soil::video::event::WindowEvent>,
                   public hook::EventHook<input::Event> {
 public:
  explicit Root(glm::ivec2 windowSize);
  ~Root() override = default;
  void OnEvent(const input::Event& event) override;
  void OnEvent(const soil::video::event::WindowEvent& event) override;

  bool IsOnElement(glm::ivec2 pos);

 protected:
  void OnStageChanged(soil::stage::Stage* stage,
                      soil::stage::Stage* prevStage) override;
};
}  // namespace soil::stage::scene::gui

#endif
