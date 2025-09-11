
#ifndef SOIL_VIDEO_RENDER_RENDERABLE_H
#define SOIL_VIDEO_RENDER_RENDERABLE_H
#include "state.h"

namespace soil::video::render {
    class Renderable {
    public:
        Renderable();
        virtual ~Renderable() = default;
        [[nodiscard]] virtual int GetContainerStateIndex() const;
        virtual void SetContainerStateIndex(int index);

        virtual void Render(State& state) = 0;

    private:
        int containerStateIndex_;
    };
} // namespace soil::video::render
#endif
