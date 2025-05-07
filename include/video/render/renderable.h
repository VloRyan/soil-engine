
#ifndef SOIL_VIDEO_RENDER_RENDERABLE_H
#define SOIL_VIDEO_RENDER_RENDERABLE_H

namespace soil::video::render {
    class Renderable {
    public:
        virtual ~Renderable() = default;

        virtual void Render() = 0;

        [[nodiscard]] virtual bool IsOpaque() const = 0;
    };
} // namespace soil::video::render
#endif // SOIL_VIDEO_RENDER_RENDERABLE_H
