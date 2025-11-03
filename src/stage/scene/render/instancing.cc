#include "stage/scene/render/instancing.h"

#include <ranges>
#include <stdexcept>
#include <utility>

#include "stage/scene/component/instance_data.h"

namespace soil::stage::scene::render {
Instancing::Instancing(
    video::render::Container* renderContainer,
    std::function<BatchObject*(const BatchObjectCreationArgs& args)>
        creatorFunc)
    : hook::TriggerHook({TriggerHook::TriggerType::AfterUpdateScene}),
      renderContainer_(renderContainer),
      batchObjectCreatorFunc_(std::move(creatorFunc)) {}

Instancing::~Instancing() {
  for (const auto& states : renderBatchPerKey_ | std::views::values) {
    delete states.Opaque;
    delete states.NonOpaque;
  }
}
void Instancing::OnEvent(const event::Node& event) {
  if (event.ChangeType != event::Node::ChangeType::Component) {
    return;
  }
  Handle(event.ComponentEvent);
}

void Instancing::Handle(const event::Component& event) {
  if (!component::VisualComponent::IsRenderType(event.Origin,
                                                render::Type::Instancing)) {
    return;
  }
  auto* data = dynamic_cast<component::InstanceData*>(event.Origin);
  switch (event.Trigger) {
    case event::Component::TriggerType::Added:
      OnAdded(data);
      break;
    case event::Component::TriggerType::Removed:
      OnRemoved(data);
      break;
    case event::Component::TriggerType::Changed:
      if (event.Changed == event::Component::ChangeType::Data) {
        OnChanged(data);
      }
      break;
    default:;
  }
}

void Instancing::OnAdded(component::InstanceData* data) {
  if (!data->IsVisible() || data->IsCulled()) {
    return;
  }

  const auto key = data->GetBatchKey();
  auto itr = renderBatchPerKey_.find(key);
  if (itr == renderBatchPerKey_.end()) {
    throw std::runtime_error("Batch with key '" + key + "' unknown");
  }
  for (auto addedDataItr = addedData_.begin(); addedDataItr != addedData_.end();
       ++addedDataItr) {
    if (data == *addedDataItr) {
      return;
    }
  }
  addedData_.push_back(data);
}

void Instancing::OnChanged(component::InstanceData* data) {
  if (!data->IsVisible() || data->IsCulled()) {
    OnRemoved(data);
    return;
  }
  if (data->GetIndex() == -1) {
    OnAdded(data);
    return;
  }
  const auto key = data->GetBatchKey();
  auto itr = renderBatchPerKey_.find(key);
  if (itr == renderBatchPerKey_.end()) {
    throw std::runtime_error("Batch with key '" + key + "' unknown");
  }
  const auto& states = itr->second;
  if (data->IsOpaque()) {
    if (states.NonOpaque != nullptr) {
      if (states.NonOpaque->RemoveInstance(data)) {
        data->SetIndex(-1);
        OnAdded(data);
        return;
      }
    }
    if (states.Opaque == nullptr) {
      OnAdded(data);
      return;
    }
    if (data->GetIndex() == -1) {  // properly just added
      return;
    }
    states.Opaque->AddChangedInstance(data);
  } else {
    if (states.Opaque != nullptr) {
      if (states.Opaque->RemoveInstance(data)) {
        data->SetIndex(-1);
        OnAdded(data);
        return;
      }
    }
    if (states.NonOpaque == nullptr) {
      OnAdded(data);
      return;
    }
    if (data->GetIndex() == -1) {  // properly just added
      return;
    }
    states.NonOpaque->AddChangedInstance(data);
  }
  // TODO remove batch if empty
}

void Instancing::OnRemoved(component::InstanceData* data) {
  if (data->GetIndex() == -1) {
    for (auto itr = addedData_.begin(); itr != addedData_.end(); ++itr) {
      if (data == *itr) {
        addedData_.erase(itr);
        break;
      }
    }
    return;
  }
  const auto key = data->GetBatchKey();
  auto itr = renderBatchPerKey_.find(key);
  if (itr == renderBatchPerKey_.end()) {
    throw std::runtime_error("Batch with key '" + key + "' unknown");
  }
  const auto& states = itr->second;
  auto removed = false;
  if (data->IsOpaque()) {
    if (states.Opaque != nullptr) {
      removed = states.Opaque->RemoveInstance(data);
    }
  } else {
    if (states.NonOpaque != nullptr) {
      removed = states.NonOpaque->RemoveInstance(data);
    }
  }
  if (!removed) {
    throw std::runtime_error("Failed to remove instance with key: " + key);
  }
  // TODO remove batch if empty
}
void Instancing::OnTrigger(
    soil::stage::hook::TriggerHook::TriggerType trigger) {
  for (auto* data : addedData_) {
    auto& states = renderBatchPerKey_[data->GetBatchKey()];
    if (data->IsOpaque()) {
      if (states.Opaque == nullptr) {
        states.Opaque = createBatch(data->GetBatchKey());
        addedOpaqueBatches_.push_back(states.Opaque);
      }
      states.Opaque->AddNewInstance(data);
    } else {
      if (states.NonOpaque == nullptr) {
        states.NonOpaque = createBatch(data->GetBatchKey());
        addedNonOpaqueBatches_.push_back(states.NonOpaque);
      }
      states.NonOpaque->AddNewInstance(data);
    }
  }
  addedData_.clear();
  for (const auto& states : renderBatchPerKey_ | std::views::values) {
    // TODO: maybe Opaque and NonOpaque can be always existing - avoid if's
    if (states.Opaque != nullptr) {
      states.Opaque->Update();
    }
    if (states.NonOpaque != nullptr) {
      states.NonOpaque->Update();
    }
  }
  for (auto* batch : addedOpaqueBatches_) {
    renderContainer_->Add(batch, video::render::Container::OPAQUE);
  }
  addedOpaqueBatches_.clear();

  for (auto* batch : addedNonOpaqueBatches_) {
    renderContainer_->Add(batch, video::render::Container::NON_OPAQUE);
  }
  addedNonOpaqueBatches_.clear();

  for (auto* batch : removedOpaqueBatches_) {
    renderContainer_->Remove(batch, video::render::Container::OPAQUE);
  }
  removedOpaqueBatches_.clear();

  for (auto* batch : removedNonOpaqueBatches_) {
    renderContainer_->Remove(batch, video::render::Container::NON_OPAQUE);
  }
  removedNonOpaqueBatches_.clear();
}

void Instancing::AddRenderBatch(const std::string& key,
                                const BatchObjectCreationArgs& args) {
  if (batchCreationArgs_.contains(key)) {
    throw std::runtime_error("Batch with key '" + key + "' already exists");
  }
  if (args.Mesh == nullptr) {
    throw std::invalid_argument("mesh can not be NULL");
  }
  batchCreationArgs_[key] = args;
  renderBatchPerKey_[key] = StateBatches();
}

Instancing::StateBatches* Instancing::GetStateBatches(const std::string& name) {
  auto itr = renderBatchPerKey_.find(name);
  if (itr != renderBatchPerKey_.end()) {
    return &itr->second;
  }
  return nullptr;
}

Instancing::BatchObject* Instancing::createBatch(const std::string& key) {
  auto itr = batchCreationArgs_.find(key);
  if (itr == batchCreationArgs_.end()) {
    throw std::runtime_error("Batch with key '" + key + "' does not exist");
  }
  return batchObjectCreatorFunc_(itr->second);
}

Instancing::BatchObject::BatchObject(video::render::Batch* batch)
    : batch_(batch) {
#ifdef DEBUG
  if (batch_ == nullptr) {
    throw std::invalid_argument("batch shall not be NULL");
  }
#endif
}
Instancing::BatchObject::BatchObject() : batch_(nullptr) {}

float Instancing::BatchObject::DistanceTo(const glm::vec3& point) {
  return 0.F;
}

void Instancing::BatchObject::Render(video::render::State& state) {
  batch_->Render(state);
}

void Instancing::BatchObject::AddChangedInstance(
    video::render::instance::Instance* instance) {
  batch_->AddChangedInstance(instance);
}
void Instancing::BatchObject::AddNewInstance(
    video::render::instance::Instance* instance) {
  batch_->AddNewInstance(instance);
}
bool Instancing::BatchObject::RemoveInstance(
    video::render::instance::Instance* instance) {
  return batch_->RemoveInstance(instance);
}
void Instancing::BatchObject::Update() { batch_->Update(); }

bool Instancing::BatchObject::IsSortable() const { return false; }

Instancing::BatchObject* Instancing::DefaultCreateBatchObject(
    const Instancing::BatchObjectCreationArgs& args) {
  GLsizei instanceSize = 0;
  for (const auto [Elements, Type] : args.VertexAttribDescriptors) {
    instanceSize +=
        video::vertex::AttributePointer::GetSizeOfDataType(Type) * Elements;
  }
  return new BatchObject(new video::render::Batch({
      .MeshData = args.Mesh,
      .Shader = args.Shader,
      .InstanceSize = instanceSize,
      .PreserveInstances = args.PreserveInstances,
      .DrawMode = args.Mesh->GetDrawMode(),
      .VertexAttribDescriptors = args.VertexAttribDescriptors,
      .State = args.State,
  }));
}
}  // namespace soil::stage::scene::render
