#ifndef SOIL_STAGE_SCENE_COMPONENT_EVENT_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_EVENT_COMPONENT_H

#include "stage/scene/component/event/hook.h"

namespace soil::stage::scene::component::event {
class EventComponent : public Component, public Hook {
 public:
  struct EventComponentDefinition {
    std::vector<EventType> Events = {};
    std::vector<soil::stage::hook::TriggerHook::TriggerPoint> TriggerPoints =
        {};
    bool GlobalNodeEvents = false;
  };
  explicit EventComponent(Type type,
                          const EventComponentDefinition &definition);
  ~EventComponent() override = default;
  
 protected:
  void OnStageChanged(soil::stage::Stage *stage,
                      soil::stage::Stage *prevStage) override;
  void SetParent(Node *parent) override;

 private:
  bool globalNodeEvents_;
};
}  // namespace soil::stage::scene::component::event
#endif
