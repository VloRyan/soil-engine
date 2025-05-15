#include "shader.h"

#include <video/shader/shader.h>

namespace soil_samples::instancing {
    Shader::Shader(const std::string& path) : soil::video::shader::Shader(NAME, path) {}

    void Shader::SetViewer(soil::stage::scene::viewer::Node* const viewer) {
        viewer_ = viewer;
    }

    void Shader::Prepare() {
        soil::video::shader::Shader::Prepare();
        SetUniform("ProjectionViewMatrix", viewer_->GetProjectionMatrix() * viewer_->GetViewMatrix());
    }

} // namespace soil_samples::instancing
