#include "input/event_map.h"

#include <utility>

namespace input {
EventMap::EventMap() = default;

EventMap::~EventMap() = default;

void EventMap::AddKeyMapping(Keys key, Event::State state,
                             const std::function<void(const Event &)> &action) {
  keyMappings_.emplace_back(key, state, action);
}

void EventMap::Handle(const Event &event) {
  switch (event.GetOrigin()) {
  case Event::Origin::Keyboard:
    if (event.GetCause() == Event::Cause::StateChanges) {
      for (const auto &mapping : keyMappings_) {
        if (mapping.Matches(event)) {
          mapping.fun_(event);
        }
      }
    }
    break;
  case Event::Origin::MouseButton:
    for (const auto &mapping : mouseButtonMappings_) {
      if (mapping.Matches(event)) {
        mapping.fun_(event);
      }
    }
    break;
  case Event::Origin::MouseWheel:
    for (const auto &mapping : mouseWheelMappings_) {
      if (mapping.Matches(event)) {
        mapping.fun_(event);
      }
    }
    break;
  }
}

void EventMap::AddMouseButtonMapping(
    const MouseButton button, const Event::State state,
    const std::function<void(const Event &)> &action) {
  mouseButtonMappings_.emplace_back(button, state, action);
}

KeyEventMapping::KeyEventMapping(const Keys key, const Event::State state,
                                 std::function<void(const Event &)> action)
    : key_(key), state_(state), fun_(std::move(action)) {}

bool KeyEventMapping::Matches(const Event &event) const {
  return event.GetKey() == key_ && event.GetState() == state_;
}

MouseButtonEventMapping::MouseButtonEventMapping(
    const MouseButton button, const Event::State state,
    std::function<void(const Event &)> action)
    : button_(button), state_(state), fun_(std::move(action)) {}

bool MouseButtonEventMapping::Matches(const Event &event) const {
  return event.GetMouseButton() == button_ && event.GetState() == state_;
}

void EventMap::AddMouseWheelMapping(
    const std::function<void(const Event &)> &action) {
  mouseWheelMappings_.emplace_back(action);
}

MouseWheelEventMapping::MouseWheelEventMapping(
    std::function<void(const Event &)> action)
    : fun_(std::move(action)) {}

bool MouseWheelEventMapping::Matches(const Event &event) { return true; }
} // namespace input
