#ifndef SOIL_STAGE_EVENTS_NODE_EVENT_H
#define SOIL_STAGE_EVENTS_NODE_EVENT_H

#include "event/event.hpp"
#include "event/handler.hpp"

namespace soil::stage::scene::component {
class Component;
}  // namespace soil::stage::scene::component

namespace soil::stage::event {

struct Component : soil::event::Event {
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
  explicit Component(stage::scene::component::Component* origin = nullptr,
                     TriggerType trigger = TriggerType::Changed,
                     ChangeType changed = ChangeType::None);
  ~Component() override = default;
  friend bool operator==(const Component& lhs, const Component& rhs) {
    return static_cast<const soil::event::Event&>(lhs) ==
               static_cast<const soil::event::Event&>(rhs) &&
           lhs.Origin == rhs.Origin && lhs.Trigger == rhs.Trigger &&
           lhs.Changed == rhs.Changed;
  }

  friend bool operator!=(const Component& lhs, const Component& rhs) {
    return !(lhs == rhs);
  }

  static Component MakeDataChangedEvent(scene::component::Component* origin,
                                        int ChangeDetail = -1);

  static Component MakeUpdateTypeChangedEvent(
      scene::component::Component* origin);

  scene::component::Component* Origin{nullptr};
  TriggerType Trigger{TriggerType::Changed};
  ChangeType Changed{ChangeType::None};
  int ChangeDetail{-1};
};

using ComponentEventHandler = soil::event::Handler<Component>;
}  // namespace soil::stage::event
#endif
