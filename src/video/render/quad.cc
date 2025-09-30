
#include "video/render/quad.h"

namespace soil::video::render {
Quad::Quad(const mesh::Data& mesh, const QuadShaderDef& shaderDef,
           const bool opaque)
    : VertexArray(mesh),
      opaque_(opaque),
      shader_(shaderDef.Shader),
      indexCount_(static_cast<int>(mesh.GetIndexCount())),
      indexType_(mesh.GetIndexType()),
      textureUniform_(shaderDef.TextureUniform),
      texture_(nullptr) {}

void Quad::Render(State& state) {
  shader_->Use();
  if (texture_ != nullptr) {
    state.SetTexture(0, *texture_);
    shader_->SetUniform(textureUniform_, 0);
  }
  VertexArray::Render(state);
}

bool Quad::IsOpaque() const { return opaque_; }

void Quad::SetTexture(texture::Texture* const texture) { texture_ = texture; }
}  // namespace soil::video::render
