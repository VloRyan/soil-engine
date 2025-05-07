
#include "video/render/batch.h"

#include "exception.h"
#include "video/buffer/vbo.h"
#include "video/render/instance/sorted_buffer.h"
#include "video/vertex/vao_creator.h"

namespace soil::video::render {
    Batch::Batch(const mesh::Mesh &mesh, shader::InstanceShader *shader, const bool opaque,
                 const int preserveInstances) :
        instanceBuffer_(nullptr), shader_(shader),
        vao_(vertex::VaoCreator::Create(mesh.GetVertexDataType(), mesh.GetVertexData(), mesh.GetVertexDataSize(),
                                        static_cast<int>(mesh.GetVertexCount()))),
        indexCount_(mesh.GetIndexCount()), indexType_(mesh.GetIndexType()), opaque_(opaque) {
        const auto instanceSize = shader->GetInstanceSize();
        const auto bufferSize = preserveInstances * instanceSize;
        buffer::Object *instanceVbo = new buffer::Vbo(buffer::Object::UsageType::Dynamic);
        instanceVbo->SetData(nullptr, static_cast<int>(bufferSize));

        size_t offset = 0;
        for (auto [Elements, Type] : shader->GetInstanceVertexDescriptors()) {
            offset = vao_->AddAttributePointer(instanceVbo, Type, Elements, static_cast<GLsizei>(instanceSize), offset,
                                               true);
        }
        vao_->Create(mesh.GetIndices(), mesh.GetIndexSize());
        if (opaque) {
            instanceBuffer_ =
                new instance::SortedBuffer(instanceVbo, instanceSize, instance::SortedBuffer::SortType::FrontToBack);
        } else {
            instanceBuffer_ =
                new instance::SortedBuffer(instanceVbo, instanceSize, instance::SortedBuffer::SortType::BackToFront);
        }
    }

    Batch::~Batch() {
        delete vao_;
        delete instanceBuffer_;
    }

    void Batch::AddChangedInstance(instance::Instance *instance) const {
        instanceBuffer_->AddChangedInstance(instance);
    }

    void Batch::AddNewInstance(instance::Instance *instance) const { instanceBuffer_->AddInstance(instance); }

    bool Batch::RemoveInstance(instance::Instance *instance) const { return instanceBuffer_->RemoveInstance(instance); }

    void Batch::Update(const glm::vec3 &viewerPos) const { instanceBuffer_->Update(viewerPos); }

    void Batch::Render() {
        const auto amount = instanceBuffer_->GetPreparedInstancesCount();
        if (amount == 0) {
            return;
        }
#ifdef DEBUG
        if (const buffer::Object *perInstanceBuffer = instanceBuffer_->GetPerInstanceBuffer();
            perInstanceBuffer != nullptr && !perInstanceBuffer->IsUsage(buffer::Object::UsageType::Dynamic)) {
            throw engine::Exception("Invalid buffer for instance drawing");
        }
#endif
        vao_->Bind();
        shader_->Use();
        shader_->PrepareRender();
        shader::Shader::DrawElementsInstanced(static_cast<int>(shader_->GetDrawMode()), indexCount_, indexType_,
                                              static_cast<int>(amount), 0);
    }

    bool Batch::IsOpaque() const { return opaque_; }

    std::string Batch::MakeKey(const mesh::Mesh &mesh, const shader::InstanceShader *shader, const bool opaque) {
        return std::to_string(mesh.GetId()) + "_" + shader->GetName() + "_" + std::to_string(opaque);
    }
} // namespace soil::video::render
