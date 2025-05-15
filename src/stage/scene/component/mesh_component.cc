#include "stage/scene/component/mesh_component.h"

namespace soil::stage::scene::component {
    MeshComponent::MeshComponent(const video::mesh::Data& mesh, video::shader::Shader* shader, const bool opaque) :
        VisualComponent(render::Type::Plain, opaque), vertexArray_(new video::render::VertexArray(mesh)),
        shader_(shader) {}

    MeshComponent::~MeshComponent() {
        delete vertexArray_;
    }

    void MeshComponent::BuildVaoFrom(const video::mesh::Data& mesh) {
        vertexArray_ = new video::render::VertexArray(mesh);
    }

    video::render::Renderable* MeshComponent::GetRenderable() {
        return this;
    }

    void MeshComponent::Render() {
        PrepareRender();
        shader_->Prepare();
        vertexArray_->Render();
    }

    video::shader::Shader* MeshComponent::GetShader() const {
        return shader_;
    }


} // namespace soil::stage::scene::component
