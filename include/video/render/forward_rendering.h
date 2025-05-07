#ifndef SOIL_VIDEO_RENDER_FORWARD_RENDERING_H_
#define SOIL_VIDEO_RENDER_FORWARD_RENDERING_H_

#include <vector>
#include "technique.h"

namespace soil::video::render {
    class ForwardRendering final : public Technique {
    public:
        ForwardRendering();

        ~ForwardRendering() override;

        void Render(State &state, const std::vector<Renderable *> &renderables) const override;
    };
} // namespace soil::video::render

#endif // SOIL_VIDEO_RENDER_FORWARD_RENDERING_H_
