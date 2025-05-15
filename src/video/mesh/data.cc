#include "video/mesh/data.h"
#include <algorithm>
#include <plog/Log.h>
#include <utility>
#include "GL/gl3w.h"

namespace soil::video::mesh {
    Data::Data() :
        indexType_(vertex::IndexType::TYPE_USHORT), indexCount_(0), indices_(nullptr),
        drawMode_(shader::DrawMode::Triangles), dataSize_(0), data_(nullptr) {}

    Data::~Data() {
        PLOG_DEBUG << "Deleting mesh " << id_;
        delete[] indices_;
        delete[] data_;
    }

    std::string Data::GetId() const {
        return id_;
    }

    uint Data::GetIndex(const uint idx) const {
        switch (indexType_) {
        case vertex::IndexType::TYPE_UINT:
            return reinterpret_cast<uint *>(indices_)[idx];
        case vertex::IndexType::TYPE_USHORT:
            return reinterpret_cast<ushort *>(indices_)[idx];
        default:
            throw std::runtime_error("unknown index type");
        }
    }

    void Data::SetIndices(const ushort *indices, const uint indexCount) {
        delete[] indices_;
        indices_ = nullptr;
        indexCount_ = indexCount;
        indexType_ = vertex::IndexType::TYPE_USHORT;
        const auto indexSize = indexCount * sizeof(GLushort);
        if (indices != nullptr) {
            indices_ = new byte[indexSize];
            memcpy(indices_, indices, indexSize);
        }
    }

    void Data::SetIndices(const uint *indices, const uint indexCount) {
        delete[] indices_;
        indices_ = nullptr;
        indexCount_ = indexCount;
        indexType_ = vertex::IndexType::TYPE_UINT;
        const auto indexSize = indexCount * sizeof(GLuint);
        if (indices != nullptr) {
            indices_ = new byte[indexSize];
            memcpy(indices_, indices, indexSize);
        }
    }

    void Data::SetData(const byte *data, const size_t dataSize,
                       const std::vector<vertex::VertexAttribDescriptor> &vertexAttribs) {
        if (dataSize != dataSize_) {
            delete[] data_;
            data_ = nullptr;
            if (data != nullptr && dataSize > 0) {
                data_ = new byte[dataSize];
            }
        }
        if (data != nullptr && dataSize > 0) {
            std::memcpy(data_, data, dataSize);
        }
        dataSize_ = dataSize;
        vertexAttribs_ = vertexAttribs;
    }

    vertex::IndexType Data::GetIndexType() const {
        return indexType_;
    }

    byte *Data::GetIndices() const {
        return indices_;
    }

    byte *Data::GetData() const {
        return data_;
    }

    gl_size_t Data::GetDataSize() const {
        return dataSize_;
    }

    shader::DrawMode Data::GetDrawMode() const {
        return drawMode_;
    }

    void Data::SetDrawMode(const shader::DrawMode drawMode) {
        drawMode_ = drawMode;
    }

    const std::vector<vertex::VertexAttribDescriptor> &Data::GetVertexAttribs() const {
        return vertexAttribs_;
    }

    GLsizei Data::GetVertexSize() const {
        GLsizei size = 0;
        for (const auto [Elements, Type] : vertexAttribs_) {
            size += vertex::AttributePointer::GetSizeOfDataType(Type) * Elements;
        }
        return size;
    }

    uint Data::GetIndexCount() const {
        return indexCount_;
    }

} // namespace soil::video::mesh
