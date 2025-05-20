#include "stage/scene/volume/container.h"

namespace soil::stage::scene::volume {
    Container::Container(world::volume::Container *container) : container_(container) {}

    Container::~Container() {
        delete container_;
    }

    void Container::ComponentAdded(component::Component *component) {
        if (component->GetType() != component::Component::Type::BoundingVolume) {
            return;
        }
        const auto *volume = dynamic_cast<component::BoundingVolume *>(component);
        if (volume == nullptr) {
            return;
        }
        container_->Insert(volume);
    }

    void Container::ComponentChanged(component::Component *component) {
        if (component->GetType() != component::Component::Type::BoundingVolume) {
            return;
        }
        const auto *volume = dynamic_cast<component::BoundingVolume *>(component);
        if (volume == nullptr) {
            return;
        }
        if (container_->Remove(volume)) {
            container_->Insert(volume);
        }
    }

    void Container::ComponentRemoved(component::Component *component) {
        if (component->GetType() != component::Component::Type::BoundingVolume) {
            return;
        }
        const auto *volume = dynamic_cast<component::BoundingVolume *>(component);
        if (volume == nullptr) {
            return;
        }
        container_->Remove(volume);
    }

    void Container::Update() {}

    void Container::ForEachAt(const glm::vec3 pos,
                              const std::function<bool(const component::BoundingVolume *volume)> &f) const {
        for (const auto *treeNode : container_->FindNodesAt(pos)) {
            for (const auto *volume : treeNode->Contained) {
                if (const auto ret = f(dynamic_cast<const component::BoundingVolume *>(volume)); !ret) {
                    return;
                }
            }
        }
    }

    const world::volume::Container *Container::GetContainer() const {
        return container_;
    }
} // namespace soil::stage::scene::volume
