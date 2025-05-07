
#ifndef SOIL_STAGE_SCENE_COMPONENT_RENDERABLE_INSTANCE_H
#define SOIL_STAGE_SCENE_COMPONENT_RENDERABLE_INSTANCE_H

#include "renderable.hpp"
#include "video/mesh/mesh.h"
#include "video/shader/instance_shader.h"


namespace soil::stage::scene::component {
    class RenderableInstance : public Renderable, public video::render::instance::Instance {
    public:
        RenderableInstance(video::mesh::Mesh *mesh, video::shader::InstanceShader *shader, bool opaque);

        [[nodiscard]] video::mesh::Mesh *GetMesh() const override;

        [[nodiscard]] video::shader::InstanceShader *GetShader() const override;

        [[nodiscard]] bool IsOpaque() const override;

        [[nodiscard]] std::string GetBatchKey() const;

        void SetBatchKey(const std::string &batchKey);

    protected:
        void SetOpaque(bool opaque);

    private:
        video::mesh::Mesh *mesh_;
        video::shader::InstanceShader *shader_;
        bool opaque_;
        std::string batchKey_;
    };
} // namespace soil::stage::scene::component


#endif // SOIL_STAGE_SCENE_COMPONENT_RENDERABLE_INSTANCE_H
