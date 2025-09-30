
#include "shape_tile_shader.h"

namespace soil_samples::gui {
ShapeTileShader::ShapeTileShader(const std::string& path)
    : Shader(NAME, path), viewer_(nullptr) {}

void ShapeTileShader::Prepare(soil::video::render::State& state) {
  Shader::Prepare(state);
  SetUniform("ProjectionViewMatrix",
             viewer_->GetProjectionMatrix() * viewer_->GetViewMatrix());
}

void ShapeTileShader::SetViewer(
    soil::stage::scene::viewer::Node* const viewer) {
  viewer_ = viewer;
}

}  // namespace soil_samples::gui
