#ifndef SOIL_VIDEO_RENDER_DRAW_DRAWABLE_HPP
#define SOIL_VIDEO_RENDER_DRAW_DRAWABLE_HPP
#include "event/observable.hpp"
#include "video/event/drawable_event.h"
#include "video/render/state_identifier.hpp"
namespace soil::video::render::draw {
class DrawableContainer;

class Drawable : public soil::event::Observable<event::DrawableEvent> {
 public:
  ~Drawable() override = default;
  [[nodiscard]] virtual const StateIdentifier& StateId() const = 0;
  virtual void Bind(State& state) = 0;
  virtual void Draw() = 0;
  virtual float DistanceTo(const glm::vec3& point) = 0;
  virtual bool IsSortable() = 0;

  DrawableContainer* Container() { return container_; };
  friend class DrawableContainer;

 protected:
  Drawable() = default;

 private:
  DrawableContainer* container_{nullptr};
};
}  // namespace soil::video::render::draw
#endif
