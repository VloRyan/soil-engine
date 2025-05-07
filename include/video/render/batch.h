
#ifndef SOIL_VIDEO_RENDER_BATCH_H
#define SOIL_VIDEO_RENDER_BATCH_H
#include <video/shader/instance_shader.h>

#include "instance/buffer.h"
#include "renderable.h"
#include "video/mesh/mesh.h"
#include "video/vertex/vao.h"

namespace soil::video::render {
    class Batch final : public Renderable {
    public:
        Batch(const mesh::Mesh &mesh, shader::InstanceShader *shader, bool opaque, int preserveInstances = 10000);

        ~Batch() override;

        void AddChangedInstance(instance::Instance *instance) const;

        void AddNewInstance(instance::Instance *instance) const;

        bool RemoveInstance(instance::Instance *instance) const;

        void Update(const glm::vec3 &viewerPos) const;

        void Render() override;

        [[nodiscard]] bool IsOpaque() const override;

        static std::string MakeKey(const mesh::Mesh &mesh, const shader::InstanceShader *shader, bool opaque);

    private:
        instance::Buffer *instanceBuffer_;
        shader::InstanceShader *shader_;
        vertex::Vao *vao_;
        uint indexCount_;
        vertex::IndexType indexType_;
        bool opaque_;
    };
} // namespace soil::video::render

#endif // SOIL_VIDEO_RENDER_BATCH_H
