#ifndef ENGINE_VIDEO_RENDER_FORWARD_RENDERING_H_
#define ENGINE_VIDEO_RENDER_FORWARD_RENDERING_H_

#include <vector>
#include "technique.h"

namespace video::render {
    class ForwardRendering : public Technique {
    public:
        ForwardRendering();

        ~ForwardRendering();

        void Render(State &state, const std::vector<Renderable *> &renderables) const override;

    private:
        //static void toSortedList(std::vector<render::object::Instance *> &instances, std::vector<Pile> &piles);

        // static bool SortByOpaqueAndRenderObject(object::Instance *a, object::Instance *b);
    };
} // Render

#endif //ENGINE_VIDEO_RENDER_FORWARD_RENDERING_H_
