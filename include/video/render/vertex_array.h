#ifndef SOIL_VIDEO_RENDER_VERTEX_ARRAY_H
#define SOIL_VIDEO_RENDER_VERTEX_ARRAY_H
#include "renderable.h"
#include "video/buffer/vbo.h"
#include "video/mesh/data.h"
#include "video/shader/shader.h"
#include "video/vertex/vao.h"

namespace soil::video::render {
    class VertexArray : public Renderable {
    public:
        explicit VertexArray(const mesh::Data &mesh);
        ~VertexArray() override;
        void Render() override;
        [[nodiscard]] vertex::Vao *GetVao() const;

    private:
        vertex::Vao *vao_;
        buffer::Vbo *vbo_;
        shader::DrawMode drawMode_;
    };
} // namespace soil::video::render

#endif // SOIL_VIDEO_RENDER_VERTEX_ARRAY_H
