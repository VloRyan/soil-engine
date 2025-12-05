#ifndef SOIL_VIDEO_EVENT_DRAWABLE_EVENT_H
#define SOIL_VIDEO_EVENT_DRAWABLE_EVENT_H

#include "event/event.hpp"
#include "event/handler.hpp"
#include "video/render/state_identifier.hpp"

namespace soil::video::render::draw {
class Drawable;
}

namespace soil::video::event {
struct DrawableEvent final : soil::event::Event {
  enum CauseType : std::uint8_t { StateChanged };

  explicit DrawableEvent(render::draw::Drawable* drawable,
                         DrawableEvent::CauseType cause);
  ~DrawableEvent() override = default;
  static DrawableEvent MakeStateChangedEvent(render::draw::Drawable* drawable,
                                             const render::StateDef& prevState);
  render::draw::Drawable* Drawable;
  CauseType Cause;
  render::StateDef PrevState;
};

using DrawableEventHandler = soil::event::Handler<DrawableEvent>;
}  // namespace soil::video::event
#endif
