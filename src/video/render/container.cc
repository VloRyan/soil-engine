#include "video/render/container.h"

namespace soil::video::render {
    void Container::Add(Renderable* renderable, const RenderDef& state) {
        const auto& renderables = renderablesPerState[state];
        const auto nextId = static_cast<int>(renderables.size());
        renderable->SetContainerStateIndex(nextId);
        renderablesPerState[state].push_back(renderable);
    }

    bool Container::Remove(Renderable* renderable, const RenderDef& state) {
        const auto index = renderable->GetContainerStateIndex();
        auto& renderables = renderablesPerState[state];
        if (renderables[index] != renderable) {
            return false;
        }
        renderables.erase(renderables.begin() + index);
        renderable->SetContainerStateIndex(-1);
        for (auto i = index; i < renderables.size(); ++i) {
            renderables[i]->SetContainerStateIndex(i);
        }
        return true;
    }

    const std::vector<Renderable*>& Container::GetPerDef(const RenderDef state) {
        return renderablesPerState[state];
    }
} // namespace soil::video::render
