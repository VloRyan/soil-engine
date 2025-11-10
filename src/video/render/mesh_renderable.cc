#include "video/render/mesh_renderable.h"

#include <utility>

#include "video/render/renderable_group.h"
#include "video/render/renderable_group_elements.h"
#include "video/vertex/vao.h"
namespace soil::video::render {
MeshRenderable::MeshRenderable(
    const mesh::Data& mesh, shader::Shader* shader,
    std::function<void(const data::IWriter& writer,
                       soil::video::render::State& state)>
        applyDataFunc)
    : Renderable({
          .Shader = shader,
          .DrawMode = mesh.GetDrawMode(),
      }),
      vao_(nullptr),
      drawMode_(mesh.GetDrawMode()),
      applyDataFunc_(std::move(applyDataFunc)),
      renderFunc_(nullptr) {
  vao_ = new vertex::Vao();
  auto usage = buffer::Object::UsageType::Static;
  if (mesh.GetData() == nullptr) {
    usage = buffer::Object::UsageType::Dynamic;
  }
  auto vbo = vao_->SetBuffer("data", new buffer::Vbo(usage));
  vbo->SetData(mesh.GetData(), mesh.GetDataSize());
  size_t offset = 0;
  const auto vertexSize = mesh.GetVertexSize();
  for (const auto [Elements, Type] : mesh.GetVertexAttribs()) {
    offset =
        vao_->AddAttributePointer("data", Type, Elements, vertexSize, offset);
  }
  vao_->CreateWithEbo(mesh.GetIndices(), mesh.GetIndexType(),
                      mesh.GetIndexCount());
  stateId_.VAOId = vao_->GetId();
}

MeshRenderable::~MeshRenderable() { delete vao_; }
void MeshRenderable::SetRenderFunc(
    const std::function<void(soil::video::render::State&, int)>& renderFunc) {
  renderFunc_ = renderFunc;
}
void MeshRenderable::Render(soil::video::render::State& state, int count) {
  if (renderFunc_ != nullptr) {
    renderFunc_(state, count);
    return;
  }
  const auto* ebo = vao_->GetEbo();
  vao_->Bind();
  shader::Shader::DrawElements(static_cast<uint>(drawMode_),
                               ebo->GetIndexCount(), ebo->GetIndexType());
}

const vertex::Vao* MeshRenderable::Vao() const { return vao_; }
vertex::Vao* MeshRenderable::GetVao() const { return vao_; }

RenderableGroup* MeshRenderable::NewGroup() const {
  return new RenderableGroupElements();
}

DrawMode MeshRenderable::DrawMode() const { return drawMode_; }

void MeshRenderable::ApplyData(const data::IWriter& writer,
                               soil::video::render::State& state) const {
  applyDataFunc_(writer, state);
}

}  // namespace soil::video::render
