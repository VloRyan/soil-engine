
#include "character_shader.h"

namespace soil_samples::gui {
    CharacterShader::CharacterShader(const std::string& path) : Shader(NAME, path), viewer_(nullptr) {}

    void CharacterShader::Prepare(soil::video::render::State& state) {
        Shader::Prepare(state);
        SetUniform("ProjectionViewMatrix", viewer_->GetProjectionMatrix() * viewer_->GetViewMatrix());
    }

    void CharacterShader::SetViewer(soil::stage::scene::viewer::Node* const viewer) {
        viewer_ = viewer;
    }

} // namespace soil_samples::gui
