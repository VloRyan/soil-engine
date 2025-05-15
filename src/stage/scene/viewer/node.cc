#include "stage/scene/viewer/node.h"
#include "stage/scene/space/quad_tree.h"

namespace soil::stage::scene::viewer {
    Node::Node(scene::Node *parent) : scene::Node(nullptr, Type::Viewer), viewChanged_(true) {
        SetParent(parent);
    }

    Node::Node(scene::Node *parent, const std::vector<scene::Node::ReceiverType> &receiverTypes) :
        scene::Node(nullptr, Type::Viewer, receiverTypes, ""), viewChanged_(true) {
        SetParent(parent);
    }

    bool Node::IsViewChanged() const {
        return viewChanged_;
    }

} // namespace soil::stage::scene::viewer
