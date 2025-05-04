#ifndef VIDEO_MESH_MESH_H_
#define VIDEO_MESH_MESH_H_
#include "../../base.h"
#include "video/vertex/vertex.h"
#include <glm/glm.hpp>
#include <string>

namespace video::mesh {
    enum class PrimitiveType : std::uint8_t {
        Line = 0,
        Quad,
        Box
    };

    struct Definition {
        std::string Identifier;
        PrimitiveType Type;

        std::string TypeString() const {
            switch (Type) {
                case (PrimitiveType::Line): return "Line";
                case (PrimitiveType::Quad): return "Quad";
                case (PrimitiveType::Box): return "Box";
            }
            return "";
        }

        std::string ToString() const {
            return TypeString() + "_" + Identifier;
        }
    };

    class Mesh {
    public:
        friend class Cache;

        explicit Mesh(int id, std::string name, const uint *indices, uint indexCount, const byte *data,
                      size_t dataSize,
                      size_t vertexSize,
                      vertex::VertexType targetType);

        explicit Mesh(int id, std::string name, const ushort *indices, ushort indexCount, const byte *data,
                      size_t dataSize,
                      size_t vertexSize,
                      vertex::VertexType targetType);

        virtual ~Mesh();

        [[nodiscard]] int GetId() const;

        [[nodiscard]] uint GetIndex(uint idx) const;

        /**
         * Saves a copy of indices.
         * @param indices data
         * @param indexCount count of indices to be copied
         */
        void SetIndices(const ushort *indices, uint indexCount);

        void SetIndices(const uint *indices, uint indexCount);

        void SetData(const byte *data, size_t dataSize, size_t vertexSize, vertex::VertexType targetType);

        [[nodiscard]] uint GetIndexCount() const;

        [[nodiscard]] glm::vec3 GetMinCoords() const;

        [[nodiscard]] glm::vec3 GetMaxCoords() const;

        /**
         * Returns the width, height and depth of the mesh calculated with min/max Coords.
         * @return dimensions x,y,z of the mesh
         */
        [[nodiscard]] glm::vec3 GetDimension() const;

        [[nodiscard]] byte* GetIndices() const;

        [[nodiscard]] vertex::IndexType GetIndexType() const;

        void SetIndexType(vertex::IndexType type);

        /**
         * Return the size in bytes of the index data
         * @return size in bytes
         */
        [[nodiscard]] uint GetIndexSize() const;

        [[nodiscard]] vertex::VertexType GetVertexDataType() const;

        [[nodiscard]] byte* GetVertexData() const;

        [[nodiscard]] uint GetVertexCount() const;

        [[nodiscard]] uint GetVertexDataSize() const;

        [[nodiscard]] uint GetMaterialIndex() const;

        [[nodiscard]] std::string GetName() const;


        static Mesh* New(int id, std::string name, PrimitiveType type);

    protected:
        void clearIndices();

        void clearData();


        int id_;
        std::string name_;

        vertex::IndexType indexType_;
        uint indexSize_;
        byte *indices_;
        uint indexCount_;

        vertex::VertexType vertexDataType_;
        byte *vertexData_;
        uint vertexCount_;
        uint vertexDataSize_;

        uint materialIndex_;
        glm::vec3 minCoords_;
        glm::vec3 maxCoords_;
    };
} // video::mesh

#endif //VIDEO_MESH_MESH_H_
