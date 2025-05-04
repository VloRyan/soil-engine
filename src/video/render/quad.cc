
#include "video/render/quad.h"

#include "video/vertex/vao_creator.h"

namespace video::render {
Quad::Quad(const mesh::Mesh &mesh, const QuadShaderDef &shaderDef,
           const bool opaque)
    : opaque_(opaque), shader_(shaderDef.Shader),
      vao_(vertex::VaoCreator::Create(
          mesh.GetVertexDataType(), mesh.GetVertexData(),
          mesh.GetVertexDataSize(), mesh.GetVertexCount())),
      indexCount_(mesh.GetIndexCount()), indexType_(mesh.GetIndexType()),
      textureUniform_(shaderDef.TextureUniform) {
  vao_->Create(mesh.GetIndices(), mesh.GetIndexSize());
}

Quad::~Quad() { delete vao_; }

void Quad::Render() {
  vao_->Bind();
  shader_->Use();
  if (texture_ != nullptr) {
    shader_->SetTexture2d(0, texture_, textureUniform_);
  }
  shader::Shader::DrawElements(GL_TRIANGLES, indexCount_, indexType_);
}

bool Quad::IsOpaque() const { return opaque_; }

void Quad::SetTexture(texture::Texture *const texture) { texture_ = texture; }
} // namespace video::render
