
#ifndef ENGINE_VIDEO_RENDER_RENDERABLE_H
#define ENGINE_VIDEO_RENDER_RENDERABLE_H

namespace video::render {
    class Renderable {
    public:
        virtual ~Renderable() = default;

        virtual void Render() = 0;

        [[nodiscard]] virtual bool IsOpaque() const = 0;
    };
}
#endif //ENGINE_VIDEO_RENDER_RENDERABLE_H
