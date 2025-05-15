#ifndef SOIL_VIDEO_RENDER_STEP_CONTEXT_H_
#define SOIL_VIDEO_RENDER_STEP_CONTEXT_H_
#include "video/render/container.h"
#include "video/render/properties.h"
#include "video/render/state.h"
#include "video/render/technique.h"

namespace soil::video::render::step {
    struct Context {
        render::Properties *Properties{nullptr};
        render::State *State{nullptr};
        render::Container *Container{nullptr};
        std::unordered_map<std::string, std::vector<Renderable *> *> NamedRenderables{};
    };
} // namespace soil::video::render::step

#endif // SOIL_VIDEO_RENDER_STEP_CONTEXT_H_
