#include "video/render/draw/instance_data_buffer.h"

#include <stdexcept>

namespace soil::video::render::draw {
InstanceDataBuffer::InstanceDataBuffer(buffer::Object* buffer,
                                       const size_t instanceSize)
    : buffer_(buffer),
      instanceSize_(instanceSize),
      dataWriter_(new data::BufferWriter(buffer_->GetCursor())) {}

InstanceDataBuffer::~InstanceDataBuffer() {
  instances_.clear();
  dirtyInstances_.clear();
}

buffer::Object* InstanceDataBuffer::GetPerInstanceBuffer() const {
  return buffer_;
}

size_t InstanceDataBuffer::InstanceSize() const { return instanceSize_; }

size_t InstanceDataBuffer::InstancesCount() const { return instances_.size(); }

size_t InstanceDataBuffer::DirtyInstancesCount() const {
  return dirtyInstances_.size();
}
const std::vector<VaoElementsInstanced::Data*>& InstanceDataBuffer::Data() {
  return instances_;
}

void InstanceDataBuffer::Update() {
  if (dirtyInstances_.empty()) {
    return;
  }
  for (auto* dirtyInstance : dirtyInstances_) {
    dataWriter_->Cursor().MoveTo(instanceSize_ * dirtyInstance->index_);
    dirtyInstance->Write(*dataWriter_);
    dirtyInstance->dirty_ = false;
  }
  dirtyInstances_.clear();
  buffer_->Flush();
}

void InstanceDataBuffer::Insert(VaoElementsInstanced::Data* instance) {
  if (instance == nullptr || instance->index_ != UnsetId) {
    return;
  }
  // TODO:
  /*if (maxInstancesPerDraw == instances_.size()) {
    throw std::runtime_error("Buffer is too small");
  }*/
  instance->index_ = static_cast<int>(instances_.size());
  instance->dirty_ = true;
  instances_.push_back(instance);
  dirtyInstances_.push_back(instance);
}

bool InstanceDataBuffer::Remove(VaoElementsInstanced::Data* instance) {
  if (instance->index_ == UnsetId) {
    return false;
  }
  if (instance->dirty_) {
    for (auto itr = dirtyInstances_.begin(); itr != dirtyInstances_.end();
         ++itr) {
      if (*itr == instance) {
        dirtyInstances_.erase(itr);
        break;
      }
    }
  }
  if (instance != instances_.back()) {
    auto* filler = instances_.back();
    instances_[instance->index_] = filler;
    filler->index_ = instance->index_;
    if (!filler->dirty_) {
      filler->dirty_ = true;
      dirtyInstances_.push_back(filler);
    }
  }
  instances_.pop_back();
  instance->index_ = UnsetId;
  instance->dirty_ = false;
  return true;
}

void InstanceDataBuffer::SetDirty(VaoElementsInstanced::Data* instance) {
  if (instance->dirty_ || instance->index_ == UnsetId) {
    return;
  }
  instance->dirty_ = true;
  dirtyInstances_.push_back(instance);
}
}  // namespace soil::video::render::draw
