#ifndef ENGINE_VIDEO_RENDER_TECHNIQUE_H_
#define ENGINE_VIDEO_RENDER_TECHNIQUE_H_

#include <cstdint>
#include <vector>

#include "renderable.h"
#include "state.h"

namespace video::render {
    enum class TechniqueType : std::uint8_t {
        Forward,
        Deferred,
    };

    class Technique {
    public:
        virtual void Render(State &state, const std::vector<Renderable *> &renderables) const = 0;

        static Technique* GetTechnique(TechniqueType type);
    };
} // video::render

#endif //ENGINE_VIDEO_RENDER_TECHNIQUE_H_
