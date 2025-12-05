#include "video/event/drawable_event.h"
namespace soil::video::event {
DrawableEvent::DrawableEvent(render::draw::Drawable* drawable,
                             DrawableEvent::CauseType cause)
    : Event(Type_t::Video), Drawable(drawable), Cause(cause) {}
DrawableEvent DrawableEvent::MakeStateChangedEvent(
    render::draw::Drawable* drawable, const render::StateDef& prevState) {
  auto event = DrawableEvent(drawable, DrawableEvent::StateChanged);
  event.PrevState = prevState;
  return event;
}
}  // namespace soil::video::event
