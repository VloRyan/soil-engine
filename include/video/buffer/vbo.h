#ifndef ENGINE_ENGINE_VIDEO_BUFFER_VBO_H_
#define ENGINE_ENGINE_VIDEO_BUFFER_VBO_H_
#include "base.h"
#include "object.h"
#include "cursor.h"
#include "video/vertex/attribute_pointer.h"

namespace video::buffer {
    class Vbo : public Object {
    public:
        /**
         * Constructs a new VertexBufferObject.
         * @param usage Specifies the expected usage pattern of the data store.
         * @param access
         */
        explicit Vbo(UsageType usage = Object::UsageType::Static, AccessType access = AccessType::Draw);

        /** Default destructor */
        ~Vbo() override;

        void Flush() override;

        void Reserve(gl_size_t newBufferSize) override;

        //void AddAttributePointer(uint index, video::vertex::AttributePointer *attribPtr);

    private:
        //void lockBuffer();

        //void waitBuffer();

        bool directWrite_;
    };
} // buffer

#endif //ENGINE_ENGINE_VIDEO_BUFFER_VBO_H_
