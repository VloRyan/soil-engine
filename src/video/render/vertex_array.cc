#include "video/render/vertex_array.h"

#include "video/vertex/vao_creator.h"

namespace soil::video::render {
    VertexArray::VertexArray(const mesh::Data &mesh) : vao_(nullptr), vbo_(nullptr), drawMode_(mesh.GetDrawMode()) {
        vao_ = new vertex::Vao();
        auto usage = buffer::Object::UsageType::Static;
        if (mesh.GetData() == nullptr) {
            usage = buffer::Object::UsageType::Dynamic;
        }
        vbo_ = new buffer::Vbo(usage);
        vbo_->SetData(mesh.GetData(), mesh.GetDataSize());
        size_t offset = 0;
        const auto vertexSize = mesh.GetVertexSize();
        for (const auto [Elements, Type] : mesh.GetVertexAttribs()) {
            offset = vao_->AddAttributePointer(vbo_, Type, Elements, vertexSize, offset);
        }
        vao_->CreateWithEbo(mesh.GetIndices(), mesh.GetIndexType(), mesh.GetIndexCount());
    }

    VertexArray::~VertexArray() {
        delete vao_;
        delete vbo_;
    }

    void VertexArray::Render() {
        const auto *ebo = vao_->GetEbo();
        vao_->Bind();
        shader::Shader::DrawElements(static_cast<uint>(drawMode_), ebo->GetIndexCount(), ebo->GetIndexType());
    }

    vertex::Vao *VertexArray::GetVao() const {
        return vao_;
    }


} // namespace soil::video::render
