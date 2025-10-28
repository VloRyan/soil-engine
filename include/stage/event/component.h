#ifndef SOIL_STAGE_EVENTS_NODE_EVENT_H
#define SOIL_STAGE_EVENTS_NODE_EVENT_H

#include "event/event.h"
#include "event/handler.hpp"

namespace soil::stage::scene::component {
class Component;
}  // namespace soil::stage::scene::component

namespace soil::stage::event {
class Component final : soil::event::Event {
 public:
  enum class TriggerType : std::uint8_t {
    Added,
    Removed,
    Changed,
  };

  enum class ChangeType : std::uint8_t {
    None,
    Data,
    UpdateType,
  };

  Component(scene::component::Component* origin, TriggerType type,
            ChangeType what = ChangeType::None);

  ~Component() override = default;

  [[nodiscard]] TriggerType Trigger() const;

  [[nodiscard]] scene::component::Component* Origin() const;

  [[nodiscard]] ChangeType Changed() const;

  friend bool operator==(const Component& lhs, const Component& rhs) {
    return static_cast<const soil::event::Event&>(lhs) ==
               static_cast<const soil::event::Event&>(rhs) &&
           lhs.origin_ == rhs.origin_ && lhs.trigger_ == rhs.trigger_ &&
           lhs.changed_ == rhs.changed_;
  }

  friend bool operator!=(const Component& lhs, const Component& rhs) {
    return !(lhs == rhs);
  }

  static Component MakeDataChangedEvent(scene::component::Component* origin);

  static Component MakeUpdateTypeChangedEvent(
      scene::component::Component* origin);

 private:
  scene::component::Component* origin_;
  TriggerType trigger_;
  ChangeType changed_;
};

using ComponentEventHandler = soil::event::Handler<Component>;
}  // namespace soil::stage::event
#endif
