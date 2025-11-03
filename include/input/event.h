#ifndef SOIL_INPUT_EVENT_H_
#define SOIL_INPUT_EVENT_H_

#include "event/event.hpp"
#include "event/handler.hpp"
#include "glm/vec2.hpp"
#include "input/constants.hpp"

namespace soil::input {
struct Event final : event::Event {
  enum class OriginType {
    Keyboard,
    MouseButton,
    MouseWheel,
    MousePosition,
  };

  enum class CauseType {
    StateChanges,
    Entered,
  };

  enum class StateType {
    Release,
    Press,
    Repeat,
  };

  ~Event() override;

  static Event MakeKeyChangedEvent(Keys key, StateType state);

  static Event MakeCharacterEnteredEvent(char character);

  static Event MakeMouseButtonEvent(glm::ivec2 pos, MouseButton button,
                                    StateType state);

  static Event MakeMouseWheelEvent(glm::ivec2 pos, glm::vec2 offset);

  static Event MakeMousePositionEvent(glm::vec2 pos);

  CauseType Cause;
  OriginType Origin;
  Keys Key;
  enum MouseButton MouseButton;
  StateType State;
  char Character;
  glm::vec2 WheelOffset;
  glm::ivec2 CursorPos;

 protected:
  Event(OriginType origin, CauseType cause);
};

using EventHandler = event::Handler<Event>;
}  // namespace soil::input

#endif
