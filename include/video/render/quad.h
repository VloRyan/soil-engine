
#ifndef SOIL_VIDEO_RENDER_QUAD_H
#define SOIL_VIDEO_RENDER_QUAD_H
#include <string>
#include "renderable.h"
#include "video/mesh/mesh.h"
#include "video/shader/instance_shader.h"
#include "video/vertex/vao.h"


namespace soil::video::render {
    class Quad final : public Renderable {
    public:
        struct QuadShaderDef {
            shader::Shader *Shader;
            std::string TextureUniform;
        };

        Quad(const mesh::Mesh &mesh, const QuadShaderDef &shaderDef, bool opaque);

        ~Quad() override;

        void Render() override;

        [[nodiscard]] bool IsOpaque() const override;

        void SetTexture(texture::Texture *texture);

    private:
        bool opaque_;
        shader::Shader *shader_;
        vertex::Vao *vao_;
        int indexCount_;
        vertex::IndexType indexType_;
        std::string textureUniform_;
        texture::Texture *texture_;
    };
} // namespace soil::video::render

#endif // SOIL_VIDEO_RENDER_QUAD_H
