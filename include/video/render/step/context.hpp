#ifndef SOIL_VIDEO_RENDER_STEP_CONTEXT_H_
#define SOIL_VIDEO_RENDER_STEP_CONTEXT_H_
#include "video/render/container.h"
#include "video/render/properties.h"
#include "video/render/state.h"

namespace soil::video::render::step {
    struct Context {
        render::Properties *Properties{nullptr};
        render::State *State{nullptr};
        render::Container *RenderContainer{nullptr};
    };
} // namespace soil::video::render::step

#endif // SOIL_VIDEO_RENDER_STEP_CONTEXT_H_
