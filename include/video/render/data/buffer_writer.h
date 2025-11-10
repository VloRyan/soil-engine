#ifndef SOIL_VIDEO_RENDER_DATA_BUFFER_WRITER_H
#define SOIL_VIDEO_RENDER_DATA_BUFFER_WRITER_H
#include "video/buffer/object.h"
#include "writer.hpp"
namespace soil::video::render::data {
class BufferWriter : public IWriter {
 public:
  explicit BufferWriter(video::buffer::Cursor& cursor);
  ~BufferWriter() override = default;
  void Write(const std::string& name, const glm::mat4& v) const override;
  video::buffer::Cursor& Cursor();
  void Write(const std::string& name, const glm::vec2& v) const override;
  void Write(const std::string& name, const glm::vec4& v) const override;
  void Write(const std::string& name, uint v) const override;
  void Write(const std::string& name, const glm::vec3& v) const override;
  void Write(const std::string& name, int v) const override;
  void Write(const std::string& name, float v) const override;
  void Write(const std::string& name, bool v) const override;

 private:
  video::buffer::Cursor& cursor_;
};
}  // namespace soil::video::render::data
#endif
