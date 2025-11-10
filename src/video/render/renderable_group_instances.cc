#include "video/render/renderable_group_instances.h"

#include "video/render/data/buffer_writer.h"
#include "video/shader/shader.h"
namespace soil::video::render {
RenderableGroupInstances::RenderableGroupInstances(
    const MeshInstancePile::PileDescriptor &pileDescriptor)
    : instanceBuffer_(nullptr),
      instanceSize_(0),
      dataWriter_(nullptr),
      dirty_(false),
      mesh_(nullptr) {
  mesh_ = new MeshRenderable(*pileDescriptor.MeshData, pileDescriptor.Shader,
                             nullptr);
  instanceBuffer_ = mesh_->GetVao()->SetBuffer(
      "instance", new buffer::Vbo(buffer::Object::UsageType::Dynamic));

  instanceSize_ = InstanceSize(pileDescriptor.VertexAttribDescriptors);
  const auto bufferSize = pileDescriptor.PreserveInstances * instanceSize_;
  instanceBuffer_->SetData(nullptr, bufferSize);

  size_t offset = 0;
  for (auto [Elements, Type] : pileDescriptor.VertexAttribDescriptors) {
    offset = mesh_->GetVao()->AddAttributePointer("instance", Type, Elements,
                                                  instanceSize_, offset, true);
  }

  dataWriter_ = new data::BufferWriter(instanceBuffer_->GetCursor());
}
RenderableGroupInstances::~RenderableGroupInstances() { delete mesh_; }

void RenderableGroupInstances::Insert(Renderable *renderable) {
  auto *instance = dynamic_cast<MeshInstance *>(renderable);
  if (instance == nullptr || isDirty(instance)) {
    return;
  }
  // TODO:
  /*if (maxInstancesPerDraw == instances_.size()) {
    throw std::runtime_error("Buffer is too small");
  }*/
  auto index = instances_.size();
  instances_.push_back(instance);
  instanceIndices_.push_back(index);
  dirtyInstanceIndices_.push_back(index);
  dirty_ = true;
}

bool RenderableGroupInstances::Remove(Renderable *instance) {
  auto index = -1;
  for (auto i = 0; i < instances_.size(); ++i) {
    if (instances_[i] == instance) {
      index = i;
      break;
    }
  }
  if (index == -1) {
    return false;
  }
  auto backIndex = instances_.size() - 1;
  if (index < backIndex) {
    instances_[index] = instances_.back();
    instanceIndices_[index] = index;

    auto instanceAlreadyDirty = false;
    for (auto &dirtyInstanceIndex : dirtyInstanceIndices_) {
      if (dirtyInstanceIndex == index) {
        instanceAlreadyDirty = true;
      }
      if (dirtyInstanceIndex == backIndex) {
        dirtyInstanceIndex = UnsetId;
        if (instanceAlreadyDirty) {
          break;
        }
      }
    }
    if (!instanceAlreadyDirty) {
      dirtyInstanceIndices_.push_back(index);
    }
  } else {
    for (auto &dirtyInstanceIndex : dirtyInstanceIndices_) {
      if (dirtyInstanceIndex == index) {
        dirtyInstanceIndex = UnsetId;
        break;
      }
    }
  }
  instances_.pop_back();
  instanceIndices_.pop_back();
  dirty_ = true;
  return true;
}

const std::vector<Renderable *> &RenderableGroupInstances::Content() const {
  auto v = std::vector<Renderable *>();
  for (auto *instance : instances_) {
    v.push_back(instance);
  }
  return v;
}

void RenderableGroupInstances::Render(video::render::State &state) {
  Update();
  const auto amount = static_cast<int>(instances_.size());
  if (amount == 0) {
#ifdef DEBUG
    throw std::runtime_error("empty group can not be rendered");
#else
    return;
#endif
  }
  auto *front = instances_.front();
  auto &stateId = front->StateId();
  state.Apply(stateId.State);
  stateId.Shader->Prepare(state);

  auto vao = mesh_->Vao();
  const auto *ebo = vao->GetEbo();
  vao->Bind();
  shader::Shader::DrawElementsInstanced(static_cast<uint>(front->DrawMode()),
                                        ebo->GetIndexCount(),
                                        ebo->GetIndexType(), amount, 0);
}

bool RenderableGroupInstances::Empty() const { return instances_.empty(); }

void RenderableGroupInstances::Update() {
  if (!dirty_ || instances_.empty()) {
    return;
  }
  State s = {};
  for (auto i : dirtyInstanceIndices_) {
    if (i == UnsetId) {
      continue;
    }
    auto index = instanceIndices_[i];
    dataWriter_->Cursor().MoveTo(instanceSize_ * index);
    instances_[index]->ApplyData(*dataWriter_, s);
  }
  dirtyInstanceIndices_.clear();
  instanceBuffer_->Flush();
  dirty_ = false;
}

bool RenderableGroupInstances::isDirty(const Renderable *instance) {
  for (auto index : dirtyInstanceIndices_) {
    if (index == UnsetId) {
      continue;
    }
    if (instances_.at(index) == instance) {
      return true;
    }
  }
  return false;
}

size_t RenderableGroupInstances::GetInstancesCount() const {
  return instances_.size();
}

size_t RenderableGroupInstances::GetDirtyInstancesCount() const {
  return dirtyInstanceIndices_.size();
}

GLsizei RenderableGroupInstances::InstanceSize(
    const std::vector<vertex::VertexAttribDescriptor> &attribs) {
  GLsizei instanceSize = 0;
  for (const auto [Elements, Type] : attribs) {
    instanceSize +=
        video::vertex::AttributePointer::GetSizeOfDataType(Type) * Elements;
  }
  return instanceSize;
}
}  // namespace soil::video::render
