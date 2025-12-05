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

  struct {
    int UploadData{0};
  } Calls;
  MockCursor Cursor_;
  void ResetMocks() { Calls.UploadData = 0; };

 protected:
  virtual void uploadData(const void* data, gl_size_t dataSize) override {
    Calls.UploadData++;
  }
};
}  // namespace soil::video::buffer
#endif
