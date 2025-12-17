#ifndef SOIL_VIDEO_SHADER_FILE_LOADER_H
#define SOIL_VIDEO_SHADER_FILE_LOADER_H
#include <string>
namespace soil::video::shader {
class FileLoader {
 public:
  FileLoader(std::string baseDir, std::string includeDir);
  ~FileLoader() = default;
  [[nodiscard]] std::string Load(const std::string& fileName) const;
  [[nodiscard]] const std::string& BaseDir() const;
  [[nodiscard]] const std::string& IncludeDir() const;

 private:
  [[nodiscard]] std::string replacePlaceholder(const std::string& line) const;
  [[nodiscard]] std::string include(const std::string& fileName) const;

  std::string baseDir_;
  std::string includeDir_;
};
}  // namespace soil::video::shader
#endif
