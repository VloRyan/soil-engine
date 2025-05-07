
#include "stage/scene/grid_scene.h"

#include "stage/scene/space/quad_tree.h"

namespace soil::stage::scene {
    GridScene::GridScene(Stage *stage, const float size, const int maxLevel, const float minChildSize) :
        Scene(stage), quadTree_(new space::QuadTree(size, maxLevel, minChildSize)) {}

    void GridScene::NodeAdded(Node *node) {
        Scene::NodeAdded(node);
        if (quadTree_ != nullptr) {
            const auto *volume = dynamic_cast<component::BoundingVolume *>(
                node->GetComponent(component::Component::Type::BoundingVolume));
            if (volume != nullptr) {
                quadTree_->Insert(volume);
            }
        }
    }

    void GridScene::NodeRemoved(const Node *node) {
        Scene::NodeRemoved(node);
        if (quadTree_ != nullptr) {
            const auto *volume = dynamic_cast<component::BoundingVolume *>(
                node->GetComponent(component::Component::Type::BoundingVolume));
            if (volume != nullptr) {
                quadTree_->Remove(volume);
            }
        }
    }

    space::QuadTree *GridScene::GetQuadTree() const { return quadTree_; }

    void GridScene::ForNodesAt(const glm::vec2 pos,
                               const std::function<void(const component::BoundingVolume *volume)> &f) const {
        for (const auto *treeNode : quadTree_->FindNodesAt(pos)) {
            for (const auto *volume : treeNode->Contained) {
                f(volume);
            }
        }
    }

    void GridScene::ComponentStateChanged(component::Component *component) {
        Scene::ComponentStateChanged(component);
        if (component->GetState() != component::Component::State::Normal || quadTree_ == nullptr) {
            return;
        }
        if (component->GetType() == component::Component::Type::BoundingVolume) {
            if (const auto *volume = dynamic_cast<component::BoundingVolume *>(component); quadTree_->Remove(volume)) {
                quadTree_->Insert(volume);
            }
        }
    }

    void GridScene::ComponentAdded(component::Component *component) {
        Scene::ComponentAdded(component);
        if (quadTree_ == nullptr) {
            return;
        }
        if (component->GetType() == component::Component::Type::BoundingVolume) {
            const auto *volume = dynamic_cast<component::BoundingVolume *>(component);
            quadTree_->Insert(volume);
        }
    }

    void GridScene::ComponentRemoved(component::Component *component) {
        Scene::ComponentRemoved(component);
        if (quadTree_ == nullptr) {
            return;
        }
        if (component->GetType() == component::Component::Type::BoundingVolume) {
            const auto *volume = dynamic_cast<component::BoundingVolume *>(component);
            quadTree_->Remove(volume);
        }
    }
} // namespace soil::stage::scene
