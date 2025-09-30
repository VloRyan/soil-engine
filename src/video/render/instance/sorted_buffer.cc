
#include "video/render/instance/sorted_buffer.h"

#include <algorithm>
#include <stdexcept>

namespace soil::video::render::instance {
SortedBuffer::SortedBuffer(buffer::Object *buffer, const size_t instanceSize,
                           const SortType sortType)
    : Buffer(buffer, instanceSize), sortType_(sortType), refPos(0.F) {}

void SortedBuffer::Update(const glm::vec3 &viewerPos) {
  if (dirtyInstances_.empty()) {
    return;
  }

  buffer::Object *perInstanceBuffer = this->GetPerInstanceBuffer();
  if (perInstanceBuffer == nullptr) {
    return;
  }

  const auto instanceSize = this->GetInstanceSize();
  const auto bufferSizeInInstances =
      perInstanceBuffer->GetBufferSize() / instanceSize;
  const auto maxInstancesPerDraw = bufferSizeInInstances;

  auto *cursor = perInstanceBuffer->GetCursor();

  for (auto *instance : dirtyInstances_) {
    if (instance->GetIndex() == -1) {
      if (maxInstancesPerDraw == instances_.size()) {
        throw std::runtime_error("Buffer is too small");
      }
      instances_.push_back(instance);
    }
    instance->SetIndex(-1);
  }

  std::vector<std::pair<float, Instance *>> sortedInstances;
  sortedInstances.reserve(instances_.size());
  // TODO sort
  /*for (auto *instance : instances_) {
      // sortedInstances.emplace_back(instance->DistanceTo(viewerPos),
  instance);
  }*/
  if (sortType_ == SortType::BackToFront) {
    std::ranges::sort(sortedInstances, SortByDistanceBackToFront);
  } else {
    std::ranges::sort(sortedInstances, SortByDistanceFrontToBack);
  }

  for (auto i = 0; i < instances_.size(); ++i) {
    auto *instance = sortedInstances[i].second;
    instances_[i] = instance;
    if (instance->GetIndex() != i) {
      cursor->MoveTo(instanceSize * i);
      instance->WriteData(cursor);
      instance->SetIndex(i);
    }
  }
  dirtyInstances_.clear();
  perInstanceBuffer->Flush();
}

glm::vec3 SortedBuffer::GetRefPos() const { return refPos; }

void SortedBuffer::SetRefPos(const glm::vec3 &ref_pos) { refPos = ref_pos; }
}  // namespace soil::video::render::instance
