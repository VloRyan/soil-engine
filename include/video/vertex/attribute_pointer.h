#ifndef SOIL_VIDEO_VERTEX_ATTRIBUTE_POINTER_H_
#define SOIL_VIDEO_VERTEX_ATTRIBUTE_POINTER_H_
#include "base.h"
#include "video/buffer/object.h"

namespace soil::video::vertex {
    class AttributePointer final {
    public:
        enum DataType : std::uint8_t { Double, Float, Int, UInt, Byte, UByte, UInt64 };

        /**
         *
         * @param vbo Specifies the Vertex Buffer Object to point to.
         * @param dataType Specifies the data type of each component in the array.
         * @param elementSize Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4. E.g.
         * one vector(x, y ,z) consists of 3 floats -> elementSize = 3
         * @param elementStride Specifies the byte offset between consecutive generic vertex attributes. If stride is 0,
         * the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
         * @param divisor Specify the number of instances_ that will pass between updates of the generic attribute. 0 =
         * per Vertex, 1 = per every Instance, 2 = per every second instance and so on
         * @param offset Specifies a offset of the first component of the first generic vertex attribute in the array in
         * the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
         */
        AttributePointer(buffer::Object *vbo, DataType dataType, int elementSize, GLsizei elementStride = 0,
                         byte divisor = 0, size_t offset = 0);

        ~AttributePointer() = default;

        void Set(uint index) const;

        [[nodiscard]] DataType GetDataType() const;

        [[nodiscard]] size_t GetOffset() const;

        [[nodiscard]] bool IsPerInstance() const;

        [[nodiscard]] byte GetDivisor() const;

        [[nodiscard]] GLsizei GetElementStride() const;

        [[nodiscard]] int GetElementSize() const;

        void SetDataType(DataType DataType);

        [[nodiscard]] uint GetGLDataType() const;

        [[nodiscard]] buffer::Object *GetVbo() const;

        void SetNormalize(bool Normalize);

        [[nodiscard]] bool IsNormalize() const;

        [[nodiscard]] static size_t GetSizeOfDataType(DataType type);

    private:
        buffer::Object *vbo_;
        DataType dataType_;
        int elementSize_;
        GLsizei elementStride_;
        byte divisor_;
        size_t offset_;
        bool normalize_;
    };
} // namespace soil::video::vertex

#endif // SOIL_VIDEO_VERTEX_ATTRIBUTE_POINTER_H_
