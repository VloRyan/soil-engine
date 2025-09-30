#include "video/buffer/ebo.h"

namespace soil::video::buffer {
Ebo::Ebo(const vertex::IndexType indexType, const UsageType usage,
         const AccessType access)
    : Object(Types::ElementArray, usage, access),
      indexType_(indexType),
      indexCount_(0) {
  Object::create();
}

Ebo::~Ebo() = default;

vertex::IndexType Ebo::GetIndexType() const { return indexType_; }

void Ebo::SetData(const void* data, const gl_size_t dataSize) {
  Object::SetData(data, dataSize);
  indexCount_ = static_cast<int>(dataSize / IndexSize(indexType_));
}

int Ebo::GetIndexCount() const { return indexCount_; }

}  // namespace soil::video::buffer
