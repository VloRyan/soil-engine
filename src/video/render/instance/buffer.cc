#include "video/render/instance/buffer.h"

#include <stdexcept>

namespace soil::video::render::instance {
Buffer::Buffer(buffer::Object* buffer, const size_t instanceSize)
    : buffer_(buffer), instanceSize_(instanceSize), dirty_(false) {}

Buffer::~Buffer() {
  instances_.clear();
  dirtyInstances_.clear();
}

buffer::Object* Buffer::GetPerInstanceBuffer() const { return buffer_; }

size_t Buffer::GetInstanceSize() const { return instanceSize_; }

size_t Buffer::GetInstancesCount() const { return instances_.size(); }

size_t Buffer::GetDirtyInstancesCount() const { return dirtyInstances_.size(); }

void Buffer::Update(const glm::vec3& viewerPos) {
  if (!dirty_) {
    return;
  }

  buffer::Object* perInstanceBuffer = this->GetPerInstanceBuffer();
  if (perInstanceBuffer == nullptr) {
    return;
  }

  const auto instanceSize = this->GetInstanceSize();
  const auto bufferSizeInInstances =
      perInstanceBuffer->GetBufferSize() / instanceSize;
  const auto maxInstancesPerDraw = bufferSizeInInstances;

  auto* cursor = perInstanceBuffer->GetCursor();

  for (auto* instance : dirtyInstances_) {
    if (instance->GetIndex() == UnsetId) {
      if (maxInstancesPerDraw == instances_.size()) {
        throw std::runtime_error("Buffer is too small");
      }
      instance->SetIndex(static_cast<int>(instances_.size()));
      instances_.push_back(instance);
    }
    cursor->MoveTo(instanceSize * instance->GetIndex());
    instance->WriteData(cursor);
  }
  dirtyInstances_.clear();
  perInstanceBuffer->Flush();
  dirty_ = false;
}

std::vector<Instance*>& Buffer::GetInstances() { return instances_; }

bool Buffer::isDirty(const Instance* instance) {
  for (auto itr = dirtyInstances_.begin(); itr != dirtyInstances_.end();
       ++itr) {
    if (*itr == instance) {
      return true;
    }
  }
  return false;
}

void Buffer::AddChangedInstance(Instance* instance) {
  if (instance->GetIndex() == UnsetId) {
    return;
  }
  if (isDirty(instance)) {
    return;
  }
  dirtyInstances_.push_back(instance);
  dirty_ = true;
}

void Buffer::PrepareInstance(Instance* instance) {
  if (isDirty(instance)) {
    return;
  }
  dirtyInstances_.push_back(instance);
  dirty_ = true;
}

bool Buffer::RemoveInstance(Instance* instance) {
  for (auto itr = dirtyInstances_.begin(); itr != dirtyInstances_.end();
       ++itr) {
    if (*itr == instance) {
      dirtyInstances_.erase(itr);
      break;
    }
  }
  for (auto i = 0; i < instances_.size(); ++i) {
    if (instances_[i] != instance) {
      continue;
    }
    if (instances_[i] != instances_.back()) {
      instances_[i] = instances_.back();
      instances_[i]->SetIndex(i);
      PrepareInstance(instances_[i]);
    }
    instance->SetIndex(UnsetId);
    instances_.pop_back();
    dirty_ = true;
    return true;
  }
  return false;
}
}  // namespace soil::video::render::instance
