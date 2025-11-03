#include "input/event_map.h"

#include <utility>

namespace soil::input {
EventMap::EventMap() = default;

EventMap::~EventMap() = default;

EventMap &EventMap::AddKeyMapping(
    Keys key, Event::StateType state,
    const std::function<void(const Event &)> &action) {
  keyMappings_.emplace_back(key, state, action);
  return *this;
}

void EventMap::Handle(const Event &event) {
  switch (event.Origin) {
    case Event::OriginType::Keyboard:
      if (event.Cause == Event::CauseType::StateChanges) {
        for (const auto &mapping : keyMappings_) {
          if (mapping.Matches(event)) {
            mapping.fun_(event);
          }
        }
      }
      break;
    case Event::OriginType::MouseButton:
      for (const auto &mapping : mouseButtonMappings_) {
        if (mapping.Matches(event)) {
          mapping.fun_(event);
        }
      }
      break;
    case Event::OriginType::MouseWheel:
      for (const auto &mapping : mouseWheelMappings_) {
        if (mapping.Matches(event)) {
          mapping.fun_(event);
        }
      }
      break;
    default:;
  }
}

EventMap &EventMap::AddMouseButtonMapping(
    const MouseButton button, const Event::StateType state,
    const std::function<void(const Event &)> &action) {
  mouseButtonMappings_.emplace_back(button, state, action);
  return *this;
}

KeyEventMapping::KeyEventMapping(const Keys key, const Event::StateType state,
                                 std::function<void(const Event &)> action)
    : key_(key), state_(state), fun_(std::move(action)) {}

bool KeyEventMapping::Matches(const Event &event) const {
  return event.Key == key_ && event.State == state_;
}

MouseButtonEventMapping::MouseButtonEventMapping(
    const MouseButton button, const Event::StateType state,
    std::function<void(const Event &)> action)
    : button_(button), state_(state), fun_(std::move(action)) {}

bool MouseButtonEventMapping::Matches(const Event &event) const {
  return event.MouseButton == button_ && event.State == state_;
}

EventMap &EventMap::AddMouseWheelMapping(
    MouseWheelDirection direction,
    const std::function<void(const Event &)> &action) {
  mouseWheelMappings_.emplace_back(direction, action);
  return *this;
}

MouseWheelEventMapping::MouseWheelEventMapping(
    const MouseWheelDirection direction,
    std::function<void(const Event &)> action)
    : direction_(direction), fun_(std::move(action)) {}

bool MouseWheelEventMapping::Matches(const Event &event) const {
  if (event.WheelOffset.y > 0.F) {
    return direction_ == MouseWheelDirection::Up;
  }
  return direction_ == MouseWheelDirection::Down;
}
}  // namespace soil::input
