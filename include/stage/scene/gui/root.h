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

  template <class T>
  T AddOverlay(T rect) {
    using type = std::remove_pointer_t<T>;
    static_assert(std::is_base_of_v<Rectangle, type>,
                  "rect must be derived from scene::Node");
    addToOverlayContainer(rect);
    return rect;
  }

  void RemoveOverlay(Rectangle* rect);
  Rectangle* FindChildAt(glm::ivec2 pos) override;

 protected:
  void OnStageChanged(soil::stage::Stage* stage,
                      soil::stage::Stage* prevStage) override;

 private:
  void addToOverlayContainer(Rectangle* rect);
  Rectangle* overlayContainer_;
};
}  // namespace soil::stage::scene::gui

#endif
