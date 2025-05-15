#ifndef ROTATION_H
#define ROTATION_H
#include <stage/scene/node.h>

namespace soil_samples::common {
    class RotationNode : public soil::stage::scene::Node {
    public:
        explicit RotationNode(Node* parent, float initialRotation);
        ~RotationNode() override = default;

        void Update() override;
    };
} // namespace soil_samples::common

#endif // ROTATION_H
