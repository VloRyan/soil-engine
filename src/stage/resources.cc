#include "stage/resources.h"

namespace soil::stage {
    Resources::Resources(Window *window, video::Manager *videoManager, sound::Manager *soundManager,
                         input::Manager *inputManager) :
        videoManager_(videoManager), soundManager_(soundManager), inputManager_(inputManager), window_(window) {}

    video::mesh::Data *Resources::GetMesh(const video::mesh::Prefab::Definition &definition) const {
        return videoManager_->GetMesh(definition);
    }

    video::shader::Shader *Resources::GetShader(const std::string &name) const {
        return videoManager_->GetShader(name);
    }

    sound::Source *Resources::GetSource(const std::string &name) const {
        return soundManager_->GetSource(name);
    }

    sound::Buffer *Resources::GetSoundBuffer(const std::string &name) const {
        return soundManager_->GetBuffer(name);
    }

    sound::Listener *Resources::GetListener() const {
        return soundManager_->GetListener();
    }

    video::texture::Texture *Resources::GetTexture2D(const std::string &fileName,
                                                     const video::texture::Parameter &parameter) const {
        return videoManager_->Texture().GetTexture2D(fileName, parameter);
    }

    video::texture::Texture *Resources::GetTextureArray2D(const std::string &fileName, const int tilesPerDim,
                                                          const video::texture::Parameter &parameter) const {
        return videoManager_->Texture().GetTextureArray2D(fileName, tilesPerDim, parameter);
    }

    Window *Resources::GetWindow() const {
        return window_;
    }
} // namespace soil::stage
