#include "video/render/data/buffer_writer.h"
namespace soil::video::render::data {
BufferWriter::BufferWriter(video::buffer::Cursor& cursor) : cursor_(cursor) {}
void BufferWriter::Write(const std::string& name, const glm::mat4& v) const {
  cursor_.Write(v);
}
void BufferWriter::Write(const std::string& name, const glm::vec2& v) const {
  cursor_.Write(v);
}
void BufferWriter::Write(const std::string& name, const glm::vec4& v) const {
  cursor_.Write(v);
}
void BufferWriter::Write(const std::string& name, const uint v) const {
  cursor_.Write(v);
}
video::buffer::Cursor& BufferWriter::Cursor() { return cursor_; }
void BufferWriter::Write(const std::string& name, const glm::vec3& v) const {
  cursor_.Write(v);
}
void BufferWriter::Write(const std::string& name, int v) const {
  cursor_.Write(v);
}
void BufferWriter::Write(const std::string& name, float v) const {
  cursor_.Write(v);
}
void BufferWriter::Write(const std::string& name, bool v) const {
  cursor_.Write(v);
}
}  // namespace soil::video::render::data
