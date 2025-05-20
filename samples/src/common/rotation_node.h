#ifndef ROTATION_H
#define ROTATION_H
#include <stage/scene/node.h>

namespace soil_samples::common {
    class RotationNode : public soil::stage::scene::Node {
    public:
        explicit RotationNode(Node* parent, float initialRotation, float velocity = 0.1F);
        ~RotationNode() override = default;

        void Update() override;

    private:
        float velocity_;
    };
} // namespace soil_samples::common

#endif // ROTATION_H
