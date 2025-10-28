#include "stage/event/component.h"

#include <glm/gtc/constants.hpp>

namespace soil::stage::event {
Component::Component(scene::component::Component* origin,
                     const TriggerType type, const ChangeType what)
    : Event(Type::Stage), origin_(origin), trigger_(type), changed_(what) {}

Component::TriggerType Component::Trigger() const { return trigger_; }

scene::component::Component* Component::Origin() const { return origin_; }

Component::ChangeType Component::Changed() const { return changed_; }

Component Component::MakeDataChangedEvent(scene::component::Component* origin) {
  return {origin, TriggerType::Changed, ChangeType::Data};
}

Component Component::MakeUpdateTypeChangedEvent(
    scene::component::Component* origin) {
  return {origin, TriggerType::Changed, ChangeType::UpdateType};
}
}  // namespace soil::stage::event
