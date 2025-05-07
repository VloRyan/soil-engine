
#include "video/vertex/vao_creator.h"

#include <stdexcept>

#include "video/buffer/vbo.h"

namespace soil::video::vertex {
    Vao *VaoCreator::Create(const VertexType type, const byte *data, const gl_size_t dataSize, const int vertexCount) {
        auto *const vao = new Vao();
        auto usage = buffer::Object::UsageType::Static;
        if (data == nullptr) {
            usage = buffer::Object::UsageType::Dynamic;
        }
        buffer::Object *vbo = new buffer::Vbo(usage);
        vbo->SetData(data, dataSize);

        const auto vertexSize = static_cast<int>(dataSize / vertexCount);
        constexpr size_t offset = 0;
        switch (type) {
        case VERT_2D:
            // Position
            vao->AddAttributePointer(vbo, AttributePointer::DataType::Float, 2, vertexSize, offset, false);
            break;
        case VERT_3D:
            // Position
            vao->AddAttributePointer(vbo, AttributePointer::DataType::Float, 3, vertexSize, offset, false);
            break;
        default:
            throw std::runtime_error("Vertex type not implemented");
        }
        return vao;
    }
} // namespace soil::video::vertex
