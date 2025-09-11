#include "stage/scene/render/plain.h"

#include <stdexcept>

#include "stage/scene/component/visual_component.h"

namespace soil::stage::scene::render {
    Plain::Plain(video::render::Container* renderContainer) :
        RenderHook(HandlerType::Component), renderContainer_(renderContainer) {}

    void Plain::Handle(const event::Component& event) {
        if (!component::VisualComponent::IsRenderType(event.GetOrigin(), render::Type::Plain)) {
            return;
        }
        auto* vComp = dynamic_cast<component::VisualComponent*>(event.GetOrigin());
        if (vComp->GetRenderable() == nullptr) {
            return;
        }
        switch (event.GetChangeType()) {
        case event::Component::ChangeType::Added:
            OnAdded(vComp);
            break;
        case event::Component::ChangeType::Removed:
            OnRemoved(vComp);
            break;
        case event::Component::ChangeType::State:
            OnStateChanged(vComp);
            break;
        }
    }

    void Plain::OnAdded(component::VisualComponent* component) {
        if (!component->IsVisible() || component->IsCulled()) {
            return;
        }
        added_.push_back(component);
    }

    void Plain::OnRemoved(component::VisualComponent* component) {
        removed_.push_back(component);
    }

    void Plain::OnStateChanged(component::VisualComponent* component) {
        if (component->IsDirty()) {
            return;
        }
        if (!component->IsVisible() || component->IsCulled()) {
            removed_.push_back(component);
            return;
        }
        if (component->GetRenderable()->GetContainerStateIndex() == -1) {
            for (const auto* comp : added_) {
                if (comp == component) {
                    return;
                }
            }
            added_.push_back(component);
        } else {
            changed_.push_back(component);
        }
    }

    void Plain::OnRender(video::render::State& state) {
        for (auto* comp : added_) {
            if (comp->IsOpaque()) {
                renderContainer_->Add(comp->GetRenderable(), video::render::Container::OPAQUE);
            } else {
                renderContainer_->Add(comp->GetRenderable(), video::render::Container::NON_OPAQUE);
            }
        }
        added_.clear();

        for (auto* comp : removed_) {
            if (comp->IsOpaque()) {
                renderContainer_->Remove(comp->GetRenderable(), video::render::Container::OPAQUE);
            } else {
                renderContainer_->Remove(comp->GetRenderable(), video::render::Container::NON_OPAQUE);
            }
        }
        removed_.clear();
        std::vector<video::render::RenderableObject*> renderables;
        for (auto* comp : changed_) {
            auto* renderable = comp->GetRenderable();
            const auto index = renderable->GetContainerStateIndex();
            if (index == -1) {
                continue;
            }
            if (comp->IsOpaque()) {
                renderContainer_->GetPerDef(renderables, video::render::Container::OPAQUE);
                if (index < renderables.size() && renderables[index] == renderable) {
                    continue; // all good
                }
                if (!renderContainer_->Remove(renderable, video::render::Container::NON_OPAQUE)) {
                    throw std::runtime_error("failed to remove renderable");
                }
                renderContainer_->Add(renderable, video::render::Container::OPAQUE);
            } else {
                renderContainer_->GetPerDef(renderables, video::render::Container::NON_OPAQUE);
                if (index < renderables.size() && renderables[index] == renderable) {
                    continue; // all good
                }
                if (!renderContainer_->Remove(renderable, video::render::Container::OPAQUE)) {
                    throw std::runtime_error("failed to remove renderable");
                }
                renderContainer_->Add(renderable, video::render::Container::NON_OPAQUE);
            }
        }
        changed_.clear();
    }
} // namespace soil::stage::scene::render
