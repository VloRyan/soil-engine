#include "input/event.h"

namespace soil::input {
    Event::Event(const Origin origin, const Cause cause) :
        event::Event(Type::Input), cause_(cause), origin_(origin), key_(Keys::Unknown), mouseButton_(MouseButton::Left),
        state_(State::Release), character_(' '), wheelOffset_(glm::vec2(0.F)) {}

    Event::~Event() = default;

    Event::Cause Event::GetCause() const { return cause_; }

    Event::Origin Event::GetOrigin() const { return origin_; }

    Keys Event::GetKey() const { return key_; }

    Event::State Event::GetState() const { return state_; }

    char Event::GetCharacter() const { return character_; }

    MouseButton Event::GetMouseButton() const { return mouseButton_; }

    glm::vec2 Event::GetWheelOffset() const { return wheelOffset_; }

    Event Event::KeyChangedEvent(const Keys key, const State state) {
        auto event = Event(Origin::Keyboard, Cause::StateChanges);
        event.key_ = key;
        event.state_ = state;
        return event;
    }

    Event Event::CharacterEnteredEvent(const char character) {
        auto event = Event(Origin::Keyboard, Cause::Entered);
        event.character_ = character;
        return event;
    }

    Event Event::MouseButtonEvent(const MouseButton button, const State state, const glm::vec2 pos) {
        auto event = Event(Origin::MouseButton, Cause::StateChanges);
        event.mouseButton_ = button;
        event.state_ = state;
        event.cursorPos_ = pos;
        return event;
    }

    Event Event::MouseWheelEvent(const glm::vec2 offset) {
        auto event = Event(Origin::MouseWheel, Cause::StateChanges);
        event.wheelOffset_ = offset;
        return event;
    }

    Event Event::MousePositionEvent(const glm::vec2 pos) {
        auto event = Event(Origin::MousePosition, Cause::StateChanges);
        event.cursorPos_ = pos;
        return event;
    }

    const glm::vec2 &Event::GetCursorPos() const { return cursorPos_; }
} // namespace soil::input
