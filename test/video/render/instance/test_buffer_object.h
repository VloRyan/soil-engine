#ifndef TEST_BUFFER_OBJECT_H
#define TEST_BUFFER_OBJECT_H
#include <cstddef>
#include <oneapi/tbb/detail/_template_helpers.h>

#include "video/buffer/object.h"
namespace soil::video::render::instance {
    class TestBufferObject final : public buffer::Object {
    public:
        explicit TestBufferObject(size_t dataSize);
        void SetData(unsigned char* data);
        void Flush() override;

    protected:
        void create() override;
    };
} // namespace soil::video::render::instance
#endif // TEST_BUFFER_OBJECT_H
