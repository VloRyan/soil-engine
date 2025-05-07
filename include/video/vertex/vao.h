#ifndef SOIL_VIDEO_VERTEX_VAO_H_
#define SOIL_VIDEO_VERTEX_VAO_H_
#include <vector>
#include "base.h"
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

        void Create(const void *indices, uint indexSize);

        void Unload();

        void Bind() const;

        static void Unbind();

        [[nodiscard]] bool IsCreated() const;

        [[nodiscard]] const std::vector<AttributePointer *> &GetAttribPointer() const;

        [[nodiscard]] buffer::Ebo *GetEbo() const;

    private:
        uint id_;
        buffer::Ebo *ebo_;
        std::vector<AttributePointer *> attribPointer_;
    };
} // namespace soil::video::vertex

#endif // SOIL_VIDEO_VERTEX_VAO_H_
