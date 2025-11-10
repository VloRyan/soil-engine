#ifndef SOIL_VIDEO_RENDER_DATA_WRITER_HPP
#define SOIL_VIDEO_RENDER_DATA_WRITER_HPP
#include <glm/glm.hpp>
#include <string>

namespace soil::video::render::data {
class IWriter {
 public:
  virtual ~IWriter() = default;
  virtual void Write(const std::string& name, const glm::mat4& v) const = 0;
  virtual void Write(const std::string& name, const glm::vec2& v) const = 0;
  virtual void Write(const std::string& name, const glm::vec3& v) const = 0;
  virtual void Write(const std::string& name, const glm::vec4& v) const = 0;
  virtual void Write(const std::string& name, int v) const = 0;
  virtual void Write(const std::string& name, uint v) const = 0;
  virtual void Write(const std::string& name, float v) const = 0;
  virtual void Write(const std::string& name, bool v) const = 0;

 protected:
  IWriter() = default;
};
}  // namespace soil::video::render::data
#endif
