#ifndef SOIL_VIDEO_VERTEX_VAO_H_
#define SOIL_VIDEO_VERTEX_VAO_H_
#include <vector>
#include "video/buffer/ebo.h"
#include "video/buffer/object.h"
#include "video/vertex/attribute_pointer.h"

namespace soil::video::vertex {
    class Vao final {
    public:
        Vao();

        ~Vao();

        [[nodiscard]] uint GetId() const;

        size_t AddAttributePointer(buffer::Object *buffer, AttributePointer::DataType dataType, int elementSize,
                                   int elementStride = 0, size_t offset = 0, bool perInstance = false);

        void CreateWithEbo(const void *indices, IndexType indexType, uint indexCount);

        void Unload();

        void Bind() const;

        static void Unbind();

        [[nodiscard]] bool IsCreated() const;

        [[nodiscard]] const std::vector<AttributePointer *> &GetAttribPointer() const;

        [[nodiscard]] buffer::Ebo *GetEbo() const;
        [[nodiscard]] buffer::Object *GetVbo() const;
        void SetVbo(buffer::Object *vbo);

    private:
        uint id_;
        buffer::Ebo *ebo_;
        buffer::Object *vbo_;
        std::vector<AttributePointer *> attribPointer_;
    };
} // namespace soil::video::vertex

#endif
