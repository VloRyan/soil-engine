#include "test_buffer_object.h"

#include "video/buffer/object.h"
namespace soil::video::render::instance {

TestBufferObject::TestBufferObject(const size_t dataSize)
    : Object(Types::Vertex, UsageType::Dynamic, AccessType::Read) {
  data_ = new byte[dataSize];
  bufferSize_ = dataSize;
}
void TestBufferObject::SetData(unsigned char* data) { data_ = data; }
void TestBufferObject::Flush() {}
void TestBufferObject::create() {}

}  // namespace soil::video::render::instance
