#ifndef SOIL_STAGE_SCENE_COMPONENT_MESH_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_MESH_COMPONENT_H
#include "video/render/vertex_array.h"
#include "visual_component.h"

namespace soil::stage::scene::component {

    class MeshComponent : public VisualComponent, public video::render::Renderable {
    public:
        MeshComponent(const video::mesh::Data& mesh, video::shader::Shader* shader, bool opaque);
        ~MeshComponent() override;
        [[nodiscard]] virtual video::shader::Shader* GetShader() const;

        [[nodiscard]] Renderable* GetRenderable() override;
        void Render() override;

    protected:
        virtual void BuildVaoFrom(const video::mesh::Data& mesh);

        virtual void PrepareRender() {}

    private:
        video::render::VertexArray* vertexArray_;
        video::shader::Shader* shader_;
    };
} // namespace soil::stage::scene::component

#endif // SOIL_STAGE_SCENE_COMPONENT_MESH_COMPONENT_H
