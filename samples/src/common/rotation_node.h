#ifndef ROTATION_NODE_H
#define ROTATION_NODE_H
#include <stage/scene/node.h>

namespace soil_samples::common {
class RotationNode : public soil::stage::scene::Node {
 public:
  explicit RotationNode(float initialRotation, float velocity = 0.1F);
  ~RotationNode() override = default;

  void Update() override;

 private:
  float velocity_;
  int ticks;
};
}  // namespace soil_samples::common

#endif
