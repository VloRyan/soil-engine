#ifndef ROTATION_NODE_H
#define ROTATION_NODE_H
#include <stage/scene/node.h>

#include "stage/hook/trigger_hook.h"

namespace soil_samples::common {
class RotationNode : public soil::stage::scene::Node,
                     public soil::stage::hook::TriggerHook {
 public:
  explicit RotationNode(float initialRotation, float velocity = 0.1F);
  ~RotationNode() override = default;

  void Update() override;
  void OnTrigger(soil::stage::hook::TriggerHook::TriggerType trigger) override;

 protected:
  void OnStageChanged(soil::stage::Stage *stage,
                      soil::stage::Stage *prevStage) override;

 private:
  float velocity_;
  int ticks;
};
}  // namespace soil_samples::common

#endif
