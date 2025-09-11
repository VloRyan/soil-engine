#include "video/render/container.h"

namespace soil::video::render {
    void Container::Add(RenderableObject* renderable, const RenderDef& state) {
        const auto nextId = static_cast<int>(renderables_.size());
        renderable->SetContainerStateIndex(nextId);
        renderables_.emplace_back(renderable, state);
    }

    bool Container::Remove(RenderableObject* renderable, const RenderDef& state) {
        const auto index = renderable->GetContainerStateIndex();
        if (index == -1 || renderables_[index].Ptr != renderable) {
            return false;
        }
        renderables_.erase(renderables_.begin() + index);
        renderable->SetContainerStateIndex(-1);
        for (auto i = index; i < renderables_.size(); ++i) {
            renderables_[i].Ptr->SetContainerStateIndex(i);
        }
        return true;
    }

    void Container::GetPerDef(std::vector<RenderableObject*>& v, const RenderDef& state) {
        for (auto& renderable : renderables_) {
            if (renderable.State == state) {
                v.push_back(renderable.Ptr);
            }
        }
    }

    bool Container::Empty() const {
        return renderables_.empty();
    }
} // namespace soil::video::render
