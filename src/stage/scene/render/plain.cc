#include "stage/scene/render/plain.h"

#include <algorithm>
#include <stdexcept>

namespace soil::stage::scene::render {
    void Plain::Update(video::render::Container* container) {
        for (auto* comp : added_) {
            if (comp->IsOpaque()) {
                container->Add(comp->GetRenderable(), video::render::Container::OPAQUE);
            } else {
                container->Add(comp->GetRenderable(), video::render::Container::NON_OPAQUE);
            }
        }
        added_.clear();

        for (auto* comp : removed_) {
            if (comp->IsOpaque()) {
                container->Remove(comp->GetRenderable(), video::render::Container::OPAQUE);
            } else {
                container->Remove(comp->GetRenderable(), video::render::Container::NON_OPAQUE);
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
                auto& renderables = container->GetPerDef(video::render::Container::OPAQUE);
                if (index < renderables.size() && renderables[index] == renderable) {
                    continue; // all good
                }
                if (!container->Remove(renderable, video::render::Container::NON_OPAQUE)) {
                    throw std::runtime_error("failed to remove renderable");
                }
                container->Add(renderable, video::render::Container::OPAQUE);
            } else {
                auto& renderables = container->GetPerDef(video::render::Container::NON_OPAQUE);
                if (index < renderables.size() && renderables[index] == renderable) {
                    continue; // all good
                }
                if (!container->Remove(renderable, video::render::Container::OPAQUE)) {
                    throw std::runtime_error("failed to remove renderable");
                }
                container->Add(renderable, video::render::Container::NON_OPAQUE);
            }
        }
        changed_.clear();
    }

    void Plain::ComponentAdded(component::VisualComponent* component) {
#ifdef DEBUG
        if (component->GetRenderType() != Type::Plain) {
            throw std::runtime_error("component is not plain render type");
        }
#endif
        if (component->GetRenderable() == nullptr) {
            return;
        }
        added_.push_back(component);
    }

    void Plain::ComponentRemoved(component::VisualComponent* component) {
#ifdef DEBUG
        if (component->GetRenderType() != Type::Plain) {
            throw std::runtime_error("component is not plain render type");
        }
#endif
        if (component->GetRenderable() == nullptr) {
            return;
        }
        removed_.push_back(component);
    }

    void Plain::ComponentChanged(component::VisualComponent* component) {
#ifdef DEBUG
        if (component->GetRenderType() != Type::Plain) {
            throw std::runtime_error("component is not plain render type");
        }
#endif
        if (component->GetRenderable() == nullptr) {
            return;
        }
        if (std::ranges::any_of(changed_, [component](const auto c) { return c == component; }) == true) {
            return;
        }
        changed_.push_back(component);
    }
} // namespace soil::stage::scene::render
