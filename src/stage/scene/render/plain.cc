#include "stage/scene/render/plain.h"
#include <stdexcept>

namespace soil::stage::scene::render {
    void Plain::Update() {
        for (auto* comp : added_) {
            if (comp->IsOpaque()) {
                container_->Add(comp->GetRenderable(), video::render::Container::OPAQUE);
            } else {
                container_->Add(comp->GetRenderable(), video::render::Container::NON_OPAQUE);
            }
        }
        added_.clear();

        for (auto* comp : removed_) {
            if (comp->IsOpaque()) {
                container_->Remove(comp->GetRenderable(), video::render::Container::OPAQUE);
            } else {
                container_->Remove(comp->GetRenderable(), video::render::Container::NON_OPAQUE);
            }
        }
        removed_.clear();

        for (auto* comp : changed_) {
            auto* renderable = comp->GetRenderable();
            const auto index = renderable->GetContainerStateIndex();
            if (index == -1) {
                continue;
            }
            if (comp->IsOpaque()) {
                auto& renderables = container_->GetPerDef(video::render::Container::OPAQUE);
                if (index < renderables.size() && renderables[index] == renderable) {
                    continue; // all good
                }
                if (!container_->Remove(renderable, video::render::Container::NON_OPAQUE)) {
                    throw std::runtime_error("failed to remove renderable");
                }
                container_->Add(renderable, video::render::Container::OPAQUE);
            } else {
                auto& renderables = container_->GetPerDef(video::render::Container::NON_OPAQUE);
                if (index < renderables.size() && renderables[index] == renderable) {
                    continue; // all good
                }
                if (!container_->Remove(renderable, video::render::Container::OPAQUE)) {
                    throw std::runtime_error("failed to remove renderable");
                }
                container_->Add(renderable, video::render::Container::NON_OPAQUE);
            }
        }
        changed_.clear();
    }

    void Plain::ComponentAdded(component::Component* component) {
        if (!IsVisualComponentOf(component, render::Type::Plain)) {
            return;
        }
        auto* vComp = dynamic_cast<component::VisualComponent*>(component);
        if (vComp->GetRenderable() == nullptr) {
            return;
        }
        added_.push_back(vComp);
    }

    void Plain::ComponentRemoved(component::Component* component) {
        if (!IsVisualComponentOf(component, Type::Plain)) {
            return;
        }
        auto* vComp = dynamic_cast<component::VisualComponent*>(component);
        if (vComp->GetRenderable() == nullptr) {
            return;
        }
        removed_.push_back(vComp);
    }

    void Plain::ComponentChanged(component::Component* component) {
        if (!component->IsDirty() || !IsVisualComponentOf(component, Type::Plain)) {
            return;
        }
        auto* vComp = dynamic_cast<component::VisualComponent*>(component);
        if (vComp->GetRenderable() == nullptr) {
            return;
        }
        if (!vComp->IsVisible() || vComp->IsCulled()) {
            removed_.push_back(vComp);
            return;
        }
        changed_.push_back(vComp);
    }
} // namespace soil::stage::scene::render
