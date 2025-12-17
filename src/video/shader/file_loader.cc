#include "video/shader/file_loader.h"

#include <fstream>
#include <utility>

#include "util/files.h"
namespace soil::video::shader {
FileLoader::FileLoader(std::string baseDir, std::string includeDir)
    : baseDir_(std::move(baseDir)), includeDir_(std::move(includeDir)) {}
std::string FileLoader::Load(const std::string& fileName) const {
  std::string line;
  std::string path = fileName;
  if (!util::Files::IsAbsolute(path)) {
    path += baseDir_;
  }
  if (!util::Files::Exists(path)) {
    throw std::runtime_error("file '" + path + "' not found");
  }
  std::ifstream in(path);
  if (!in.is_open()) {
    throw std::runtime_error("could not open file: " + path);
  }
  std::string content;
  while (std::getline(in, line)) {
    if (line.empty()) {
      content += '\n';
      continue;
    }
    content += replacePlaceholder(line) + '\n';
  }
  in.close();
  return content;
}

std::string FileLoader::replacePlaceholder(const std::string& line) const {
  if (line.starts_with("//#include ")) {
    return include(line.substr(11));
  }
  return line;
}

std::string FileLoader::include(const std::string& fileName) const {
  std::string includeFile = includeDir_ + fileName + ".glsl";
  return util::Files::Read(includeFile);
}
const std::string& FileLoader::BaseDir() const { return baseDir_; }
const std::string& FileLoader::IncludeDir() const { return includeDir_; }
}  // namespace soil::video::shader
