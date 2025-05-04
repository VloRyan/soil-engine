
#ifndef QUAD_H
#define QUAD_H
#include "renderable.h"
#include "video/mesh/mesh.h"
#include "video/shader/instance_shader.h"
#include "video/vertex/vao.h"
#include <string>


namespace video::render {
    class Quad : public Renderable {
    public:
        struct QuadShaderDef {
            shader::Shader *Shader;
            std::string TextureUniform;
        };

        Quad(const mesh::Mesh &mesh, const QuadShaderDef &shaderDef, bool opaque);

        ~Quad() override;

        void Render() override;

        [[nodiscard]] bool IsOpaque() const override;

        void SetTexture(texture::Texture *const texture);

    private:
        bool opaque_;
        shader::Shader *shader_;
        vertex::Vao *vao_;
        int indexCount_;
        vertex::IndexType indexType_;
        std::string textureUniform_;
        texture::Texture *texture_;
    };
}

#endif //QUAD_H
