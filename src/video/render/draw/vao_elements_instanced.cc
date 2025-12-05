#include "video/render/draw/vao_elements_instanced.h"

#include "video/buffer/vbo.h"
#include "video/mesh/data.h"
#include "video/render/draw/instance_data_buffer.h"
#include "video/shader/shader.h"
namespace soil::video::render::draw {
util::Cache<VaoElementsInstanced> VaoElementsInstanced::CACHE;
VaoElementsInstanced::VaoElementsInstanced(const PileDescriptor& pileDescriptor,
                                           StateDef state)
    : stateId_({
          .Shader = pileDescriptor.Shader,
          .Vao = nullptr,
          .State = state,
      }),
      drawMode_(pileDescriptor.MeshData->GetDrawMode()),
      instanceBuffer_(nullptr) {
  auto* vao = vertex::Vao::NewFrom(*pileDescriptor.MeshData);

  auto instanceBuffer = vao->SetBuffer(
      "instance", new buffer::Vbo(buffer::Object::UsageType::Dynamic));

  auto instanceSize = InstanceSize(pileDescriptor.VertexAttribDescriptors);
  const auto bufferSize = pileDescriptor.PreserveInstances * instanceSize;
  instanceBuffer->SetData(nullptr, bufferSize);

  size_t offset = 0;
  for (auto [Elements, Type] : pileDescriptor.VertexAttribDescriptors) {
    offset = vao->AddAttributePointer("instance", Type, Elements, instanceSize,
                                      offset, true);
  }
  instanceBuffer_ = new InstanceDataBuffer(instanceBuffer, instanceSize);
  stateId_.Vao = vao;
}

const StateIdentifier& VaoElementsInstanced::StateId() const {
  return stateId_;
}

void VaoElementsInstanced::Draw() {
  Update();
  const auto amount = static_cast<int>(instanceBuffer_->InstancesCount());
  if (amount == 0) {
#ifdef DEBUG
    throw std::runtime_error("empty group can not be rendered");
#else
    return;
#endif
  }

  auto* ebo = stateId_.Vao->GetEbo();
  shader::Shader::DrawElementsInstanced(static_cast<uint>(drawMode_),
                                        ebo->GetIndexCount(),
                                        ebo->GetIndexType(), amount, 0);
}

float VaoElementsInstanced::DistanceTo(const glm::vec3& point) { return 0; }

void VaoElementsInstanced::Bind(State& state) {
  state.Apply(stateId_.State);
  state.SetShader(stateId_.Shader);
  stateId_.Shader->Prepare(state);
  state.BindVao(stateId_.Vao);
}

void VaoElementsInstanced::Update() { instanceBuffer_->Update(); }

GLsizei VaoElementsInstanced::InstanceSize(
    const std::vector<vertex::VertexAttribDescriptor>& attribs) {
  GLsizei instanceSize = 0;
  for (const auto [Elements, Type] : attribs) {
    instanceSize +=
        video::vertex::AttributePointer::GetSizeOfDataType(Type) * Elements;
  }
  return instanceSize;
}

void VaoElementsInstanced::Insert(Data* instance) {
  instanceBuffer_->Insert(instance);
}

bool VaoElementsInstanced::Remove(Data* instance) {
  return instanceBuffer_->Remove(instance);
}

void VaoElementsInstanced::SetDirty(Data* instance) {
  instanceBuffer_->SetDirty(instance);
}

bool VaoElementsInstanced::IsSortable() { return false; }

VaoElementsInstanced* VaoElementsInstanced::Prepare(
    const VaoElementsInstanced::PileDescriptor& pileDescriptor,
    StateDef state) {
  auto* pile = new VaoElementsInstanced(pileDescriptor, state);
  CACHE.Put(pileDescriptor.Name, pile);
  return pile;
}

VaoElementsInstanced* VaoElementsInstanced::GetPile(const std::string& name) {
  return CACHE.Get(name);
}

}  // namespace soil::video::render::draw
