
#include "video/render/batch.h"

#include <plog/Log.h>

#include "video/buffer/vbo.h"
#include "video/render/instance/sorted_buffer.h"
#include "video/render/renderable_group.h"
#include "video/render/renderable_group_instances.h"

namespace soil::video::render {
Batch::Batch(const BatchDescriptor& descriptor)
    : Renderable({}),
      instanceVbo_(new buffer::Vbo(buffer::Object::UsageType::Dynamic)),
      instanceBuffer_(nullptr),
      shader_(descriptor.Shader),
      mesh_(
          new MeshRenderable(*descriptor.MeshData, descriptor.Shader, nullptr)),
      drawMode_(descriptor.DrawMode),
      stateDef_(descriptor.State) {
  const auto bufferSize =
      descriptor.PreserveInstances * descriptor.InstanceSize;
  instanceVbo_->SetData(nullptr, bufferSize);
  instanceBuffer_ = new instance::Buffer(instanceVbo_, descriptor.InstanceSize);
  size_t offset = 0;
  for (auto [Elements, Type] : descriptor.VertexAttribDescriptors) {
    /*offset = mesh_->Vao()->AddAttributePointer(
        instanceVbo_, Type, Elements, descriptor.InstanceSize, offset, true);*/
  }
}
Batch::Batch()
    : Renderable({}),
      instanceVbo_(nullptr),
      instanceBuffer_(nullptr),
      shader_(nullptr),
      mesh_(nullptr),
      drawMode_(DrawMode::Points) {}

Batch::~Batch() {
  PLOG_DEBUG << "Deleting Batch";
  delete mesh_;
  delete instanceVbo_;
  delete instanceBuffer_;
}

void Batch::AddChangedInstance(instance::Instance* instance) const {
  instanceBuffer_->AddChangedInstance(instance);
}

void Batch::AddNewInstance(instance::Instance* instance) const {
  instance->SetBatch(this);
  instanceBuffer_->PrepareInstance(instance);
}

bool Batch::RemoveInstance(instance::Instance* instance) const {
  return instanceBuffer_->RemoveInstance(instance);
}

void Batch::Update() const { instanceBuffer_->Update(); }

void Batch::Render(soil::video::render::State& state, int count) {
  const auto amount = instanceBuffer_->GetInstancesCount();
  if (amount == 0) {
    return;
  }
  state.Apply(stateDef_);
  const auto* vao = mesh_->Vao();
  const auto* ebo = vao->GetEbo();
  vao->Bind();
  //  shader_->Prepare(state);
  shader::Shader::DrawElementsInstanced(
      static_cast<int>(drawMode_), ebo->GetIndexCount(), ebo->GetIndexType(),
      static_cast<int>(amount), 0);
}

DrawMode Batch::DrawMode() const { return drawMode_; }

shader::Shader* Batch::GetShader() const { return shader_; }
RenderableGroup* Batch::NewGroup() const {
  return nullptr;  // new RenderableGroupInstances();
}
const vertex::Vao* Batch::Vao() const { return mesh_->Vao(); }

void Batch::ApplyData(const data::IWriter& writer,
                      soil::video::render::State& state) const {}

}  // namespace soil::video::render
