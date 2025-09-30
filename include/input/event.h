#ifndef SOIL_INPUT_EVENT_H_
#define SOIL_INPUT_EVENT_H_

#include "event/event.h"
#include "event/handler.hpp"
#include "glm/vec2.hpp"
#include "input/constants.hpp"

namespace soil::input {
class Event final : public event::Event {
 public:
  enum class Origin {
    Keyboard,
    MouseButton,
    MouseWheel,
    MousePosition,
  };

  enum class Cause { StateChanges, Entered };

  enum class State {
    Release,
    Press,
    Repeat,
  };

  ~Event() override;

  [[nodiscard]] Origin GetOrigin() const;

  [[nodiscard]] Cause GetCause() const;

  [[nodiscard]] Keys GetKey() const;

  [[nodiscard]] State GetState() const;

  [[nodiscard]] char GetCharacter() const;

  [[nodiscard]] MouseButton GetMouseButton() const;

  [[nodiscard]] glm::vec2 GetWheelOffset() const;

  [[nodiscard]] const glm::vec2& GetCursorPos() const;

  static Event MakeKeyChangedEvent(Keys key, State state);

  static Event MakeCharacterEnteredEvent(char character);

  static Event MakeMouseButtonEvent(glm::vec2 pos, MouseButton button,
                                    State state);

  static Event MakeMouseWheelEvent(glm::vec2 pos, glm::vec2 offset);

  static Event MakeMousePositionEvent(glm::vec2 pos);

 protected:
  Event(Origin origin, Cause cause);

 private:
  Cause cause_;
  Origin origin_;
  Keys key_;
  MouseButton mouseButton_;
  State state_;
  char character_;
  glm::vec2 wheelOffset_;
  glm::vec2 cursorPos_{};
};

using EventHandler = event::Handler<Event>;
}  // namespace soil::input

#endif
