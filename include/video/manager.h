#ifndef SOIL_VIDEO_MANAGER_H_
#define SOIL_VIDEO_MANAGER_H_

#include "adapter.h"
#include "mesh/cache.h"
#include "render/state.h"
#include "shader/cache.h"
#include "texture/manager.h"
#include "util/cache.hpp"
#include "window.hpp"

#define OPEN_GL_MAJOR_VERSION 3
#define OPEN_GL_MINOR_VERSION 3

namespace soil::video {
class Context;
class Manager final {
 public:
  explicit Manager(const Context::CreateParameter& contextParameter);
  ~Manager() = default;
  void Update();
  void BeginRender();
  void EndRender() const;
  [[nodiscard]] mesh::Data* GetMesh(const mesh::Prefab::Definition& definition);
  shader::Shader* GetShader(const std::string& name);
  void PrepareShader(const std::string& name, const std::string& path);
  void PrepareShader(shader::Shader* shader);
  texture::Manager& Texture();
  render::State& GetState();
  void NewUniformBufferObject(const std::string& name, gl_size_t size,
                              int target);
  class Window* GetWindow();
  Context* GetContext() const;
  bool WindowIsOpen();

  util::Cache<vertex::Vao>& VaoCache();

 protected:
  void initState();
  void InitOpenGL(const Context::CreateParameter& contextParameter);
  void InitCache();

 private:
  Context* context_;
  Window* window_;
  Adapter adapter;
  render::State* state_;
  mesh::Cache meshCache_;
  int nextMeshId;
  shader::Cache shaderCache_;
  texture::Manager textureManager_;

  util::Cache<vertex::Vao> vaoCache_;

  static void debugOutput(GLenum source, GLenum type, GLuint id,
                          GLenum severity, GLsizei length, const char* message,
                          const void* userParam);
};
}  // namespace soil::video

#endif
