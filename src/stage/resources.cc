#include "stage/resources.h"

#include "stage/manager.h"

namespace soil::stage {
Resources::Resources(video::Manager* videoManager, sound::Manager* soundManager,
                     input::Manager* inputManager)
    : videoManager_(videoManager),
      soundManager_(soundManager),
      inputManager_(inputManager) {}

video::mesh::Data* Resources::GetMesh(
    const video::mesh::Prefab::Definition& definition) const {
  return videoManager_->GetMesh(definition);
}

video::vertex::Vao* Resources::GetVao(const std::string& name) const {
  return videoManager_->VaoCache().Get(name);
}

video::shader::Program* Resources::GetShader(const std::string& name) const {
  return videoManager_->GetShader(name);
}

sound::Source* Resources::GetSource(const std::string& name,
                                    const bool loop) const {
  return soundManager_->GetSource(name, loop);
}

sound::Listener* Resources::GetListener() const {
  return soundManager_->GetListener();
}

video::render::State& Resources::GetRenderState() const {
  return videoManager_->GetState();
}

video::texture::Manager& Resources::Textures() const {
  return videoManager_->Texture();
}
video::Window* Resources::GetWindow() const {
  return videoManager_->GetWindow();
}
}  // namespace soil::stage
