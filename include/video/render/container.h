#ifndef SOIL_VIDEO_RENDER_FORWARD_CONTAINER_H
#define SOIL_VIDEO_RENDER_FORWARD_CONTAINER_H
#include <unordered_map>
#include <vector>


#include "renderable.h"

namespace soil::video::render {
    struct RenderDef {
        bool Blending{false};

        friend bool operator==(const RenderDef& lhs, const RenderDef& rhs) {
            return lhs.Blending == rhs.Blending;
        }

        friend bool operator!=(const RenderDef& lhs, const RenderDef& rhs) {
            return !(lhs == rhs);
        }
    };

    struct RenderDef_hash {
        size_t operator()(const RenderDef& o) const {
            return o.Blending;
        }
    };

    class Container {
    public:
        Container() = default;
        ~Container() = default;
        void Add(Renderable* renderable, const RenderDef& state);
        bool Remove(Renderable* renderable, const RenderDef& state);
        const std::vector<Renderable*>& GetPerDef(RenderDef state);

        inline static auto OPAQUE = RenderDef{.Blending = false};
        inline static auto NON_OPAQUE = RenderDef{.Blending = true};

    private:
        std::unordered_map<RenderDef, std::vector<Renderable*>, RenderDef_hash> renderablesPerState;
    };
} // namespace soil::video::render

#endif // SOIL_VIDEO_RENDER_FORWARD_CONTAINER_H
