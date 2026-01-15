#include "stage/event/component.h"

#include "stage/scene/component/component.h"

namespace soil::stage::event {
Component::Component(stage::scene::component::Component* origin,
                     Component::TriggerType trigger,
                     Component::ChangeType changed)
    : Event(Type_t::Stage),
      Origin(origin),
      Trigger(trigger),
      Changed(changed) {}

Component Component::MakeDataChangedEvent(scene::component::Component* origin,
                                          int changeDetail) {
  auto event = Component(origin, TriggerType::Changed, ChangeType::Data);
  event.ChangeDetail = changeDetail;
  return event;
}

Component Component::MakeUpdateTypeChangedEvent(
    scene::component::Component* origin) {
  return Component(origin, TriggerType::Changed, ChangeType::UpdateType);
}

}  // namespace soil::stage::event
