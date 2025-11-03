#include "input/event.h"

#include "sound/event/event.hpp"

namespace soil::input {
Event::Event(const OriginType origin, const CauseType cause)
    : event::Event(Type_t::Input),
      Cause(cause),
      Origin(origin),
      Key(Keys::Unknown),
      MouseButton(MouseButton::Left),
      State(StateType::Release),
      Character(' '),
      WheelOffset(glm::vec2(0.F)),
      CursorPos(glm::vec2(0.F)) {}

Event::~Event() = default;

Event Event::MakeKeyChangedEvent(const Keys key, const StateType state) {
  auto event = Event(OriginType::Keyboard, CauseType::StateChanges);
  event.Key = key;
  event.State = state;
  return event;
}

Event Event::MakeCharacterEnteredEvent(const char character) {
  auto event = Event(OriginType::Keyboard, CauseType::Entered);
  event.Character = character;
  return event;
}

Event Event::MakeMouseButtonEvent(const glm::ivec2 pos,
                                  const enum MouseButton button,
                                  const StateType state) {
  auto event = Event(OriginType::MouseButton, CauseType::StateChanges);
  event.MouseButton = button;
  event.State = state;
  event.CursorPos = pos;
  return event;
}

Event Event::MakeMouseWheelEvent(const glm::ivec2 pos, const glm::vec2 offset) {
  auto event = Event(OriginType::MouseWheel, CauseType::StateChanges);
  event.WheelOffset = offset;
  event.CursorPos = pos;
  return event;
}

Event Event::MakeMousePositionEvent(const glm::vec2 pos) {
  auto event = Event(OriginType::MousePosition, CauseType::StateChanges);
  event.CursorPos = pos;
  return event;
}

}  // namespace soil::input
// namespace soil::input
