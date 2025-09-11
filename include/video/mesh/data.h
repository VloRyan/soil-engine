#ifndef SOIL_VIDEO_MESH_MESH_H
#define SOIL_VIDEO_MESH_MESH_H
#include <string>

#include "base.h"
#include "video/shader/shader.h"
#include "video/vertex/vertex.h"

namespace soil::video::mesh {
    class Data final {
    public:
        explicit Data();

        ~Data();

        /**
         * Saves a copy of indices.
         * @param indices data
         * @param indexCount count of indices to be copied
         */
        void SetIndices(const ushort* indices, uint indexCount);

        void SetIndices(const uint* indices, uint indexCount);

        void SetData(const byte* data, size_t dataSize,
                     const std::vector<vertex::VertexAttribDescriptor>& vertexAttribs);

        [[nodiscard]] uint GetIndexCount() const;

        [[nodiscard]] byte* GetIndices() const;

        [[nodiscard]] vertex::IndexType GetIndexType() const;

        [[nodiscard]] byte* GetData() const;

        [[nodiscard]] gl_size_t GetDataSize() const;

        [[nodiscard]] shader::DrawMode GetDrawMode() const;
        void SetDrawMode(shader::DrawMode drawMode);

        [[nodiscard]] const std::vector<vertex::VertexAttribDescriptor>& GetVertexAttribs() const;
        [[nodiscard]] GLsizei GetVertexSize() const;

    private:
        // std::string id_; TODO: Needed?

        vertex::IndexType indexType_;
        uint indexCount_;
        byte* indices_;
        shader::DrawMode drawMode_;

        gl_size_t dataSize_;
        byte* data_;
        std::vector<vertex::VertexAttribDescriptor> vertexAttribs_;
    };
} // namespace soil::video::mesh

#endif
