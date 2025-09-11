#include "shader.h"

#include <video/shader/shader.h>

namespace soil_samples::line {
    Shader::Shader(const std::string& path) : soil::video::shader::Shader(NAME, path), viewer_(nullptr) {}

    void Shader::SetViewer(soil::stage::scene::viewer::Node* const viewer) {
        viewer_ = viewer;
    }

    void Shader::Prepare(soil::video::render::State& state) {
        soil::video::shader::Shader::Prepare(state);
        SetUniform("ProjectionViewMatrix", viewer_->GetProjectionMatrix() * viewer_->GetViewMatrix());
        for (auto i = 0; i < textures_.size(); i++) {
            state.SetTexture(i, *textures_[i]);
            SetUniform("Textures[" + std::to_string(i) + "]", i);
        }
    }

    void Shader::SetTextures(const std::vector<soil::video::texture::Texture*>& textures) {
        textures_ = textures;
    }
} // namespace soil_samples::line
