#include "stage/scene/volume/frustum_culling.h"

#include "stage/scene/scene.h"
#include "util/strings.h"
#include "world/volume/quad_tree.h"

namespace soil::stage::scene::volume {
    FrustumCulling::FrustumCulling(viewer::Node* viewer, const world::volume::Container* container) :
        UpdateHook(HandlerType::Component), container_(container), viewer_(viewer) {}

    void FrustumCulling::Handle(const event::Component& event) {
        if (event.GetChangeType() == event::Component::ChangeType::Added) {
            OnComponentAdded(event.GetOrigin());
        }
    }

    void FrustumCulling::OnComponentAdded(component::Component* component) {
        auto* vComp = component::VisualComponent::Cast(component);
        if (vComp == nullptr) {
            return;
        }
        addedVisualComponents_.push_back(vComp);
    }

    void FrustumCulling::OnUpdate() {
        if (nodesVisibility_.size() < container_->GetNodeCount()) {
            // TODO make it more efficient
            nodesVisibility_.resize(container_->GetNodeCount(), false);
        }
        if (viewer_ == nullptr) {
            return;
        }
        updateVisibilityOnTreeNode(0, viewer_->GetFrustum());
        for (auto* vComp : addedVisualComponents_) {
            const auto* parent = vComp->GetParent();
            if (!parent->HasComponent(component::Component::Type::BoundingVolume)) {
                continue;
            }
            const auto* volume = dynamic_cast<const component::BoundingVolume*>(
                parent->GetFirstComponent(component::Component::Type::BoundingVolume));
            std::vector<int> indices;
            container_->QueryNodeIndicesFor(volume, indices);
            auto visible = false;
            for (const auto index : indices) {
                if (nodesVisibility_[index]) {
                    visible = true;
                    break;
                }
            }
            vComp->SetCulled(!visible);
        }
        addedVisualComponents_.clear();
    }

    void FrustumCulling::updateVisibilityOnTreeNode(const int index, const world::volume::Frustum* frustum) {
        const auto* treeNode = container_->GetNode(index);
        const auto isVisible =
            frustum->IntersectBox({treeNode->Min.x, 0, treeNode->Min.y}, {treeNode->Max.x, 0, treeNode->Max.y});
        if (!isVisible && nodesVisibility_[index] == isVisible) {
            return;
        }
        if (nodesVisibility_[index] != isVisible) {
            std::vector<const world::volume::Volume*> volumes;
            container_->GetVolumes(index, volumes);
            for (const auto* c : volumes) {
                const auto* volume = dynamic_cast<const component::BoundingVolume*>(c);
                const auto* node = volume->GetParent();
                node->ForEachComponent(
                    [isVisible](component::Component* comp) {
                        auto* vComp = dynamic_cast<component::VisualComponent*>(comp);
                        vComp->SetCulled(!isVisible);
                    },
                    component::Component::Type::Visual);
            }
            nodesVisibility_[index] = isVisible;
            if (!nodesVisibility_[index]) {
                const int childrenStartIndex = treeNode->ChildrenStartIndex;
                if (childrenStartIndex == world::volume::Container::NO_CHILDREN) {
                    return;
                }
                for (int j = 0; j < container_->GetChildrenPerNode(); ++j) {
                    if (nodesVisibility_[childrenStartIndex + j] != isVisible) {
                        setVisibility(childrenStartIndex + j, false);
                    }
                }
                return;
            }
        }
        if (nodesVisibility_[index]) {
            const int childrenStartIndex = treeNode->ChildrenStartIndex;
            if (childrenStartIndex == world::volume::Container::NO_CHILDREN) {
                return;
            }
            for (int j = 0; j < container_->GetChildrenPerNode(); ++j) {
                updateVisibilityOnTreeNode(childrenStartIndex + j, frustum);
            }
        }
    }

    void FrustumCulling::setVisibility(const int index, const bool visible) {
        if (nodesVisibility_[index] == visible) {
            return;
        }
        const auto* treeNode = container_->GetNode(index);
        const int childrenStartIndex = treeNode->ChildrenStartIndex;
        if (childrenStartIndex == world::volume::Container::NO_CHILDREN) {
            return;
        }
        for (int j = 0; j < container_->GetChildrenPerNode(); ++j) {
            setVisibility(childrenStartIndex + j, visible);
        }
    }
} // namespace soil::stage::scene::volume
