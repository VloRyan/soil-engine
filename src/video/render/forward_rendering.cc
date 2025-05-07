#include "video/render/forward_rendering.h"

#include "engine.h"

namespace soil::video::render {
    ForwardRendering::ForwardRendering() = default;

    ForwardRendering::~ForwardRendering() = default;

    void ForwardRendering::Render(State &state, const std::vector<Renderable *> &renderables) const {
        auto transparent = std::vector<Renderable *>();
        transparent.reserve(renderables.size());
        for (const auto &renderable : renderables) {
            if (renderable->IsOpaque()) {
                state.SetBlend(false);
                renderable->Render();
            } else {
                transparent.emplace_back(renderable);
            }
        }
        for (const auto &renderable : transparent) {
            state.SetBlend(true);
            renderable->Render();
        }
    }
} // namespace soil::video::render
