#include "stage/resources.h"

#include "stage/manager.h"

namespace soil::stage {
    Resources::Resources(Window* window, video::Manager* videoManager, sound::Manager* soundManager,
                         input::Manager* inputManager, Manager* stageManager) :
        videoManager_(videoManager), soundManager_(soundManager), inputManager_(inputManager), window_(window),
        stageManager_(stageManager) {}

    video::mesh::Data* Resources::GetMesh(const video::mesh::Prefab::Definition& definition) const {
        return videoManager_->GetMesh(definition);
    }

    video::shader::Shader* Resources::GetShader(const std::string& name) const {
        return videoManager_->GetShader(name);
    }

    sound::Source* Resources::GetSource(const std::string& name) const {
        return soundManager_->GetSource(name);
    }

    sound::Buffer* Resources::GetSoundBuffer(const std::string& name) const {
        return soundManager_->GetBuffer(name);
    }

    sound::Listener* Resources::GetListener() const {
        return soundManager_->GetListener();
    }

    video::render::State& Resources::GetRenderState() const {
        return videoManager_->GetState();
    }

    Window* Resources::GetWindow() const {
        return window_;
    }

    video::texture::Manager& Resources::Textures() const {
        return videoManager_->Texture();
    }

    IStages& Resources::Stages() const {
        return *stageManager_;
    }
} // namespace soil::stage
