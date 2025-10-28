
#include "video/render/batch.h"

#include <plog/Log.h>

#include "video/buffer/vbo.h"
#include "video/render/instance/sorted_buffer.h"
#include "video/vertex/vao_creator.h"

namespace soil::video::render {
Batch::Batch(const BatchDescriptor& descriptor)
    : instanceVbo_(new buffer::Vbo(buffer::Object::UsageType::Dynamic)),
      instanceBuffer_(nullptr),
      shader_(descriptor.Shader),
      va_(new VertexArray(*descriptor.MeshData)),
      drawMode_(descriptor.DrawMode),
      stateDef_(descriptor.State) {
  const auto bufferSize =
      descriptor.PreserveInstances * descriptor.InstanceSize;
  instanceVbo_->SetData(nullptr, bufferSize);
  instanceBuffer_ = new instance::Buffer(instanceVbo_, descriptor.InstanceSize);
  size_t offset = 0;
  for (auto [Elements, Type] : descriptor.VertexAttribDescriptors) {
    offset = va_->GetVao()->AddAttributePointer(
        instanceVbo_, Type, Elements, descriptor.InstanceSize, offset, true);
  }
}
Batch::Batch()
    : instanceVbo_(nullptr),
      instanceBuffer_(nullptr),
      shader_(nullptr),
      va_(nullptr),
      drawMode_(shader::DrawMode::Points) {}

Batch::~Batch() {
  PLOG_DEBUG << "Deleting Batch";
  delete va_;
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

void Batch::Render(State& state) {
  const auto amount = instanceBuffer_->GetInstancesCount();
  if (amount == 0) {
    return;
  }
#ifdef DEBUG
  if (const buffer::Object* perInstanceBuffer =
          instanceBuffer_->GetPerInstanceBuffer();
      perInstanceBuffer != nullptr &&
      !perInstanceBuffer->IsUsage(buffer::Object::UsageType::Dynamic)) {
    throw std::runtime_error("Invalid buffer for instance drawing");
  }
#endif
  state.Apply(stateDef_);
  const auto* vao = va_->GetVao();
  const auto* ebo = vao->GetEbo();
  vao->Bind();
  shader_->Prepare(state);
  shader::Shader::DrawElementsInstanced(
      static_cast<int>(drawMode_), ebo->GetIndexCount(), ebo->GetIndexType(),
      static_cast<int>(amount), 0);
}

shader::DrawMode Batch::GetDrawMode() const { return drawMode_; }

shader::Shader* Batch::GetShader() const { return shader_; }

}  // namespace soil::video::render
