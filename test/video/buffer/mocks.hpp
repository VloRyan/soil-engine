#ifndef SOIL_ENGINE_VIDEO_BUFFER_MOCKS_HPP
#define SOIL_ENGINE_VIDEO_BUFFER_MOCKS_HPP
#include "video/buffer/object.h"
namespace soil::video::buffer {
class BufferObjectMock : public Object {
 public:
  class MockCursor : public Cursor {
   public:
    MockCursor() : Cursor(nullptr) {}
    void Write(const void* data, size_t dataSize) override {}
  };

  BufferObjectMock()
      : Object(Types::Vertex, UsageType::Dynamic, AccessType::Draw) {}

  Cursor& GetCursor() override { return Cursor_; }
  void Flush() override { Calls.Flush++; }
  struct {
    int Flush{0};
  } Calls;
  MockCursor Cursor_;
  void ResetMocks() { Calls.Flush = 0; };

 protected:
};
}  // namespace soil::video::buffer
#endif
