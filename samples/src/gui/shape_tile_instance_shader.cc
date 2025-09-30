
#include "shape_tile_instance_shader.h"

namespace soil_samples::gui {
ShapeTileInstanceShader::ShapeTileInstanceShader(const std::string& path)
    : Shader(NAME, path), viewer_(nullptr) {}

void ShapeTileInstanceShader::Prepare(soil::video::render::State& state) {
  Shader::Prepare(state);
  SetUniform("ProjectionViewMatrix",
             viewer_->GetProjectionMatrix() * viewer_->GetViewMatrix());
}

void ShapeTileInstanceShader::SetViewer(
    soil::stage::scene::viewer::Node* const viewer) {
  viewer_ = viewer;
}

}  // namespace soil_samples::gui
