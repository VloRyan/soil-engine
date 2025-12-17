
#ifndef SOIL_STAGE_RESOURCES_H
#define SOIL_STAGE_RESOURCES_H
#include "input/manager.h"
#include "sound/manager.h"
#include "video/manager.h"
#include "video/mesh/data.h"
#include "video/shader/program.h"

namespace soil::stage {
class Resources {
 public:
  explicit Resources(video::Manager* videoManager, sound::Manager* soundManager,
                     input::Manager* inputManager);

  ~Resources() = default;

  Resources(const Resources& other) = delete;

  Resources(Resources&& other) noexcept = delete;

  Resources& operator=(const Resources& other) = delete;

  Resources& operator=(Resources&& other) noexcept = delete;

  [[nodiscard]] virtual video::mesh::Data* GetMesh(
      const video::mesh::Prefab::Definition& definition) const;

  [[nodiscard]] video::vertex::Vao* GetVao(const std::string& name) const;

  [[nodiscard]] virtual video::shader::Program* GetShader(
      const std::string& name) const;
  [[nodiscard]] virtual sound::Source* GetSource(const std::string& name) const;

  [[nodiscard]] virtual sound::Source* GetSource(const std::string& name,
                                                 bool loop) const;

  [[nodiscard]] virtual sound::Listener* GetListener() const;

  [[nodiscard]] virtual video::Window* GetWindow() const;

  [[nodiscard]] virtual video::texture::Manager& Textures() const;

  [[nodiscard]] virtual video::render::State& GetRenderState() const;

 private:
  video::Manager* videoManager_;
  sound::Manager* soundManager_;
  input::Manager* inputManager_;
};
}  // namespace soil::stage

#endif
