#include "stage/scene/volume/frustum_culling.h"
#include "stage/scene/scene.h"
#include "util/strings.h"
#include "world/volume/quad_tree.h"

namespace soil::stage::scene::volume {
    FrustumCulling::FrustumCulling(viewer::Node *viewer, world::volume::Container *container) :
        container_(container), viewer_(viewer) {}

    void FrustumCulling::ComponentAdded(component::Component *component) {
        if (component->GetType() != component::Component::Type::Visual) {
            return;
        }
        auto *vComp = dynamic_cast<component::VisualComponent *>(component);
        if (vComp == nullptr) {
            return;
        }
        addedVisualComponents_.push_back(vComp);
    }

    void FrustumCulling::ComponentChanged(component::Component *component) {}

    void FrustumCulling::ComponentRemoved(component::Component *component) {}

    void FrustumCulling::Update() {
        if (nodesVisibility_.empty()) {
            nodesVisibility_.resize(container_->GetNodeCount(), false);
        }
        if (viewer_ == nullptr) {
            return;
        }
        updateVisibilityOnTreeNode(0, viewer_->GetFrustum());
        for (auto *vComp : addedVisualComponents_) {
            const auto *parent = vComp->GetParent();
            if (!parent->HasComponent(component::Component::Type::BoundingVolume)) {
                continue;
            }
            const auto *volume = dynamic_cast<const component::BoundingVolume *>(
                parent->GetComponent(component::Component::Type::BoundingVolume));
            const auto index = container_->FindNodeIndexFor(volume);
            if (index == -1) {
                continue;
            }
            vComp->SetCulled(!nodesVisibility_[index]);
        }
        addedVisualComponents_.clear();
    }

    void FrustumCulling::updateVisibilityOnTreeNode(const int index, const world::volume::Frustum *frustum) {
        const auto *treeNode = container_->GetNode(index);
        const auto isVisible = frustum->IntersectBox(treeNode->Min, treeNode->Max);
        if (!isVisible && nodesVisibility_[index] == isVisible) {
            return;
        }
        if (nodesVisibility_[index] != isVisible) {
            for (const auto *c : treeNode->Contained) {
                const auto *volume = dynamic_cast<const component::BoundingVolume *>(c);
                const auto *node = volume->GetParent();
                for (auto vComps = node->GetComponents(component::Component::Type::Visual); auto *comp : vComps) {
                    auto *vComp = dynamic_cast<component::VisualComponent *>(comp);
                    vComp->SetCulled(!isVisible);
                }
            }
            nodesVisibility_[index] = isVisible;
            if (!nodesVisibility_[index]) {
                const int firstChildIndex = treeNode->FirstChildIndex;
                if (firstChildIndex == -1) {
                    return;
                }
                for (int j = 0; j < container_->GetChildrenPerNode(); ++j) {
                    if (nodesVisibility_[firstChildIndex + j] != isVisible) {
                        setVisibility(firstChildIndex + j, false);
                    }
                }
                return;
            }
        }
        if (nodesVisibility_[index]) {
            const int firstChildIndex = treeNode->FirstChildIndex;
            if (firstChildIndex == -1) {
                return;
            }
            for (int j = 0; j < container_->GetChildrenPerNode(); ++j) {
                updateVisibilityOnTreeNode(firstChildIndex + j, frustum);
            }
        }
    }

    void FrustumCulling::setVisibility(const int index, const bool visible) {
        if (nodesVisibility_[index] == visible) {
            return;
        }
        const auto *treeNode = container_->GetNode(index);
        const int firstChildIndex = treeNode->FirstChildIndex;
        if (firstChildIndex == -1) {
            return;
        }
        for (int j = 0; j < container_->GetChildrenPerNode(); ++j) {
            setVisibility(firstChildIndex + j, visible);
        }
    }

    void FrustumCulling::OnAddedToScene(Scene *scene) {
        viewer_ = scene->GetViewer();
        if (viewer_ == nullptr) {
            throw std::runtime_error("Viewer can not be NULL");
        }
    }
} // namespace soil::stage::scene::volume
