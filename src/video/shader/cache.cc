#include "video/shader/cache.h"

#include <ranges>
#include <stdexcept>

namespace soil::video::shader {
Cache::~Cache() {
  for (const auto *shader : shaders_ | std::views::values) {
    delete shader;
  }
}

void Cache::ForEach(const std::function<void(Program *)> &callback) const {
  for (auto *shader : shaders_ | std::views::values) {
    callback(shader);
  }
}

Program *Cache::GetByName(const std::string &name) {
  if (const auto itr = shaders_.find(name); itr != shaders_.end()) {
    return itr->second;
  }
  return nullptr;
}

Program *Cache::GetById(const int id) const {
  for (auto *shader : shaders_ | std::views::values) {
    if (shader->GetId() == id) {
      return shader;
    }
  }
  return nullptr;
}

void Cache::Prepare(const std::string &name, const std::string &path) {
  auto *shader = GetByName(name);
  if (shader != nullptr) {
    return;
  }
  shader = new Program(name, path);
  shader->Create();
  std::pair pair(name, shader);
  shaders_.insert(pair);
}

void Cache::Prepare(const std::string &name, Program *shader) {
  if (const auto *existing = GetByName(name); existing != nullptr) {
    throw std::runtime_error("Shader already exists");
  }
  shader->Create();
  std::pair pair(name, shader);
  shaders_.insert(pair);
}

Program *Cache::Create(const std::string &name,
                       const shader::Program::Definition &programDef) {
#ifdef DEBUG
  if (shaders_.contains(name)) {
    throw std::runtime_error("Shader " + name + " already cached");
  }
#endif
  auto *shader = new Program(programDef);
  shader->SetName(name);
  std::pair pair(name, shader);
  shaders_.insert(pair);
  return shader;
}
}  // namespace soil::video::shader
