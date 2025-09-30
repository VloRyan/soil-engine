#ifndef SOIL_VIDEO_RENDER_QUAD_H
#define SOIL_VIDEO_RENDER_QUAD_H
#include <string>

#include "vertex_array.h"
#include "video/mesh/data.h"

namespace soil::video::render {
class Quad final : public VertexArray {
 public:
  struct QuadShaderDef {
    shader::Shader* Shader;
    std::string TextureUniform;
  };

  Quad(const mesh::Data& mesh, const QuadShaderDef& shaderDef, bool opaque);

  ~Quad() override = default;

  void Render(State& state) override;

  [[nodiscard]] bool IsOpaque() const;

  void SetTexture(texture::Texture* texture);

 private:
  bool opaque_;
  shader::Shader* shader_;
  int indexCount_;
  vertex::IndexType indexType_;
  std::string textureUniform_;
  texture::Texture* texture_;
};
}  // namespace soil::video::render

#endif
