
#ifndef BASIC_SHADER_H
#define BASIC_SHADER_H
#include <video/shader/shader.h>

#include <string>

#include "stage/scene/viewer/node.h"

namespace soil_samples::basic {

class Shader final : public soil::video::shader::Shader {
 public:
  explicit Shader(const std::string& path);
  ~Shader() override = default;
  inline static const std::string NAME = "Basic";
  void SetViewer(soil::stage::scene::viewer::Node* viewer);

  void Prepare(soil::video::render::State& state) override;

 private:
  soil::stage::scene::viewer::Node* viewer_;
};
}  // namespace soil_samples::basic

#endif
