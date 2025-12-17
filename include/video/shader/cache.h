#ifndef SOIL_VIDEO_SHADER_CACHE_H_
#define SOIL_VIDEO_SHADER_CACHE_H_

#include <functional>
#include <string>

#include "base.h"
#include "video/shader/program.h"

namespace soil::video::shader {
class Cache final {
 public:
  Cache() = default;

  ~Cache();

  [[nodiscard]] Program* GetByName(const std::string& name);

  [[nodiscard]] Program* GetById(int id) const;

  void Prepare(const std::string& name, const std::string& path = "");

  void Prepare(const std::string& name, Program* shader);

  [[nodiscard]] Program* Create(const std::string& name,
                                const shader::Program::Definition& programDef);

  void ForEach(const std::function<void(Program*)>& callback) const;

  Cache(Cache const&) = delete;

  void operator=(Cache const&) = delete;

 private:
  HashMap<std::string, Program*> shaders_;
};
}  // namespace soil::video::shader

#endif
