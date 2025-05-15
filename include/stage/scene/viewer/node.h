#ifndef SOIL_STAGE_SCENE_VIEWER_NODE_H
#define SOIL_STAGE_SCENE_VIEWER_NODE_H

#include "glm/glm.hpp"
#include "stage/scene/node.h"
#include "video/viewer.h"
#include "world/collision/frustum.h"

namespace soil::stage::scene::viewer {
    class Node : public scene::Node {
    public:
        ~Node() override = default;

        [[nodiscard]] virtual glm::mat4 GetViewMatrix() const = 0;

        [[nodiscard]] virtual glm::mat4 GetProjectionMatrix() const = 0;

        virtual world::collision::Frustum *GetFrustum() = 0;

        [[nodiscard]] bool IsViewChanged() const;

    protected:
        explicit Node(scene::Node *parent);
        Node(scene::Node *parent, const std::vector<scene::Node::ReceiverType> &receiverTypes);
        bool viewChanged_;
    };
} // namespace soil::stage::scene::viewer
#endif // SOIL_STAGE_SCENE_VIEWER_NODE_H
