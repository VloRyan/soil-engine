#include "video/mesh/mesh.h"
#include <algorithm>
#include <plog/Log.h>
#include <utility>
#include "GL/gl3w.h"
#include "exception.h"

namespace soil::video::mesh {
    Mesh::Mesh(const int id, std::string name, const ushort *indices, const ushort indexCount, const byte *data,
               const size_t dataSize, const size_t vertexSize, const vertex::VertexType targetType) :
        id_(id), name_(std::move(name)), indexType_(vertex::IndexType::TYPE_USHORT), indexSize_(0), indices_(nullptr),
        indexCount_(0), vertexDataType_(vertex::VertexType::VERT_2D), vertexData_(nullptr), vertexCount_(0),
        vertexDataSize_(0), materialIndex_(0), minCoords_(0.0F), maxCoords_(0.0F) {
        SetIndices(indices, indexCount);
        SetData(data, dataSize, vertexSize, targetType);
    }

    Mesh::Mesh(const int id, std::string name, const uint *indices, const uint indexCount, const byte *data,
               const size_t dataSize, const size_t vertexSize, const vertex::VertexType targetType) :
        id_(id), name_(std::move(name)), indexType_(vertex::IndexType::TYPE_USHORT), indexSize_(0), indices_(nullptr),
        indexCount_(0), vertexDataType_(vertex::VertexType::VERT_2D), vertexData_(nullptr), vertexCount_(0),
        vertexDataSize_(0), materialIndex_(0), minCoords_(0.0F), maxCoords_(0.0F) {
        SetIndices(indices, indexCount);
        SetData(data, dataSize, vertexSize, targetType);
    }

    Mesh::~Mesh() {
        PLOG_DEBUG << "Deleting mesh " << name_;
        clearIndices();
    }

    uint Mesh::GetIndex(const uint idx) const {
        switch (indexType_) {
        case vertex::IndexType::TYPE_UINT:
            return reinterpret_cast<uint *>(indices_)[idx];
        case vertex::IndexType::TYPE_USHORT:
            return reinterpret_cast<ushort *>(indices_)[idx];
        default:
            throw Exception("unknown index type");
        }
    }

    glm::vec3 Mesh::GetMinCoords() const { return minCoords_; }

    glm::vec3 Mesh::GetMaxCoords() const { return maxCoords_; }

    glm::vec3 Mesh::GetDimension() const { return maxCoords_ - minCoords_; }

    void Mesh::SetIndices(const ushort *indices, const uint indexCount) {
        indexCount_ = indexCount;
        indexType_ = vertex::IndexType::TYPE_USHORT;
        indexSize_ = indexCount * sizeof(ushort);
        if (indices_ != nullptr) {
            delete[] indices_;
            indices_ = nullptr;
        }
        if (indices != nullptr) {
            indices_ = new byte[indexSize_];
            memcpy(indices_, indices, indexSize_);
        }
    }

    void Mesh::SetIndices(const uint *indices, const uint indexCount) {
        indexCount_ = indexCount;
        indexType_ = vertex::IndexType::TYPE_UINT;
        indexSize_ = indexCount * sizeof(uint);
        if (indices != nullptr) {
            delete[] indices_;
            indices_ = new byte[indexSize_];
            memcpy(indices_, indices, indexSize_);
        }
    }

    void Mesh::SetData(const byte *data, const size_t dataSize, const size_t vertexSize,
                       const vertex::VertexType targetType) {
        clearData();
        vertexCount_ = dataSize / vertexSize;
        vertexDataType_ = targetType;
        uint targetTypeSize = 0;
        uint posComponents = 0;
        switch (targetType) {
        case vertex::VertexType::VERT_2D:
            targetTypeSize = sizeof(vertex::Vertex2d);
            posComponents = 2;
            break;
        case vertex::VertexType::VERT_3D:
            targetTypeSize = sizeof(vertex::Vertex3d);
            posComponents = 3;
            break;
        }
        vertexDataSize_ = targetTypeSize * vertexCount_;
        vertexData_ = new byte[vertexDataSize_];
        byte *dataPointer = vertexData_;
        minCoords_ = glm::vec3(std::numeric_limits<float>::max());
        maxCoords_ = glm::vec3(std::numeric_limits<float>::min());
        for (uint i = 0; i < dataSize; i += vertexSize) {
            auto &vertex = (vertex::Vertex3d &)data[i];
            std::memcpy(dataPointer, &vertex, targetTypeSize);
            dataPointer += targetTypeSize;
            for (glm::vec4::length_type j = 0; j < posComponents; ++j) {
                minCoords_[j] = std::min(vertex.m_Position[j], minCoords_[j]);
                maxCoords_[j] = std::max(vertex.m_Position[j], maxCoords_[j]);
            }
        }
    }

    void Mesh::clearIndices() {
        delete indices_;
        indices_ = nullptr;
        indexSize_ = 0;
        indexCount_ = 0;
    }

    void Mesh::clearData() {
        delete vertexData_;
        vertexData_ = nullptr;
        vertexDataSize_ = 0;
        vertexCount_ = 0;
    }

    uint Mesh::GetIndexSize() const { return indexSize_; }

    vertex::IndexType Mesh::GetIndexType() const { return indexType_; }

    byte *Mesh::GetIndices() const { return indices_; }

    vertex::VertexType Mesh::GetVertexDataType() const { return vertexDataType_; }

    byte *Mesh::GetVertexData() const { return vertexData_; }

    uint Mesh::GetVertexCount() const { return vertexCount_; }

    uint Mesh::GetVertexDataSize() const { return vertexDataSize_; }

    uint Mesh::GetMaterialIndex() const { return materialIndex_; }

    std::string Mesh::GetName() const { return name_; }

    void Mesh::SetIndexType(const vertex::IndexType type) { indexType_ = type; }

    uint Mesh::GetIndexCount() const { return indexCount_; }

    Mesh *Mesh::New(const int id, std::string name, const PrimitiveType type) {
        switch (type) {
        case PrimitiveType::Line:
            {
                constexpr float vertices[2]{0.0F, 1.0F};
                constexpr GLushort indices[2]{0, 1};
                return new Mesh(id, std::move(name), indices, 2, reinterpret_cast<const byte *>(vertices),
                                2 * sizeof(float), sizeof(float), vertex::VertexType::VERT_2D);
            }
        case PrimitiveType::Quad:
            {
                // The VBO containing the 4 vertices of the particles.
                constexpr float vertices[8]{
                    -0.5F, 0.5F, // Higher left
                    -0.5F, -0.5F, // Lower left
                    0.5F,  -0.5F, // Lower right
                    0.5F,  0.5F // Higher right
                };

                // cube indices for index buffer object
                constexpr GLushort indices[6]{0, 1, 2, 0, 2, 3};
                return new Mesh(id, std::move(name), indices, 6, reinterpret_cast<const byte *>(vertices),
                                8 * sizeof(float), 2 * sizeof(float), vertex::VertexType::VERT_2D);
            }
        case PrimitiveType::Box:
            {
                constexpr GLushort indices[36]{0, 1, 2, 3, 4, 5, 5, 0, 2, 4, 6, 0, 6, 3, 1, 2, 3, 5,
                                               0, 6, 1, 3, 7, 4, 5, 4, 0, 4, 7, 6, 6, 7, 3, 2, 1, 3};
                constexpr float boxVertices[24]{
                    1.0, -1.0, 1.0, -1.0, -1.0, -1.0, 1.0,  -1.0, -1.0, -1.0, 1.0, -1.0,
                    1.0, 1.0,  1.0, 1.0,  1.0,  -1.0, -1.0, -1.0, 1.0,  -1.0, 1.0, 1.0,
                };
                return new Mesh(id, std::move(name), indices, 36, reinterpret_cast<const byte *>(boxVertices),
                                24 * sizeof(float), 3 * sizeof(float), vertex::VertexType::VERT_3D);
            }
        default:
            throw Exception("Unknown type");
        }
    }

    int Mesh::GetId() const { return id_; }
} // namespace soil::video::mesh
