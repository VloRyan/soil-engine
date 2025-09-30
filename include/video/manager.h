#ifndef SOIL_VIDEO_MANAGER_H_
#define SOIL_VIDEO_MANAGER_H_

#include "adapter.h"
#include "mesh/cache.h"
#include "render/state.h"
#include "shader/cache.h"
#include "texture/manager.h"
#include "window.h"

namespace soil::video {
class Manager final {
 public:
  Manager();

  ~Manager() = default;

  void Init(Window* window);

  void Update();

  void BeginRender();

  void EndRender() const;

  [[nodiscard]] mesh::Data* GetMesh(const mesh::Prefab::Definition& definition);

  shader::Shader* GetShader(const std::string& name);

  shader::InstanceShader* GetIShader(const std::string& name);

  void PrepareShader(const std::string& name, const std::string& path);

  void PrepareIShader(shader::InstanceShader* shader);

  void PrepareShader(shader::Shader* shader);

  texture::Manager& Texture();

  render::State& GetState();

  void NewUniformBufferObject(const std::string& name, gl_size_t size,
                              int target);

 protected:
  void initState();

 private:
  Window* window_;
  Adapter adapter;
  render::State state_;
  mesh::Cache meshCache_;
  int nextMeshId;
  shader::Cache shaderCache_;
  texture::Manager textureManager_;

  static void debugOutput(GLenum source, GLenum type, GLuint id,
                          GLenum severity, GLsizei length, const char* message,
                          const void* userParam);
};
}  // namespace soil::video

#endif
