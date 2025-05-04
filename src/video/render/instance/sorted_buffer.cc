
#include "video/render/instance/sorted_buffer.h"

#include <algorithm>

#include "exception.h"

namespace video::render::instance {
SortedBuffer::SortedBuffer(buffer::Object *buffer, const size_t instanceSize,
                           SortType sortType)
    : Buffer(buffer, instanceSize), sortType_(sortType) {}

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
      if (maxInstancesPerDraw == preparedInstances_.size()) {
        throw engine::Exception("Buffer is too small");
      }
      preparedInstances_.push_back(instance);
    }
    instance->SetIndex(-1);
  }

  std::vector<std::pair<float, Instance *>> sortedInstances;
  sortedInstances.reserve(preparedInstances_.size());
  for (auto *instance : preparedInstances_) {
    sortedInstances.emplace_back(instance->DistanceTo(viewerPos), instance);
  }
  if (sortType_ == SortType::BackToFront) {
    std::ranges::sort(sortedInstances, SortByDistanceBackToFront);
  } else {
    std::ranges::sort(sortedInstances, SortByDistanceFrontToBack);
  }

  for (auto i = 0; i < preparedInstances_.size(); ++i) {
    auto *instance = sortedInstances[i].second;
    preparedInstances_[i] = instance;
    if (instance->GetIndex() != i) {
      cursor->MoveTo(instanceSize * i);
      instance->WriteData(cursor, i);
      instance->SetIndex(i);
    }
  }
  dirtyInstances_.clear();
  perInstanceBuffer->Flush();
}
} // namespace video::render::instance
