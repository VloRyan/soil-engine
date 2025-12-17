
#include "shader.h"

#include "video/shader/program.h"

namespace soil_samples::basic {
Shader::Shader(const std::string& path)
    : soil::video::shader::Program(NAME, path), viewer_(nullptr) {}

void Shader::SetViewer(soil::stage::scene::viewer::Node* const viewer) {
  viewer_ = viewer;
}

void Shader::Prepare(soil::video::render::State& state) {
  soil::video::shader::Program::Prepare(state);
  SetUniform("ProjectionViewMatrix",
             viewer_->GetProjectionMatrix() * viewer_->GetViewMatrix());
}
soil::stage::scene::viewer::Node* Shader::GetViewer() const { return viewer_; }

}  // namespace soil_samples::basic
