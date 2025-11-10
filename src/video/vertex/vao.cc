#include "video/vertex/vao.h"

#include "GL/gl3w.h"
#include "plog/Log.h"

namespace soil::video::vertex {
Vao::Vao() : id_(0), ebo_(nullptr) {}

Vao::~Vao() {
  Unload();
  for (const auto* ptr : attribPointer_) {
    delete ptr;
  }
  delete ebo_;
  for (auto* buffer : bufferObjects_) {
    delete buffer;
  }
}

void Vao::CreateWithEbo(const void* indices, const IndexType indexType,
                        const uint indexCount) {
  if (IsCreated()) {
    return;
  }
  glGenVertexArrays(1, &this->id_);
  this->Bind();
  const auto indexSize = IndexSize(indexType) * indexCount;
  // Element buffer
  if (indexCount > 0) {
    if (indices != nullptr) {
      ebo_ = new buffer::Ebo(indexType);
    } else {
      ebo_ = new buffer::Ebo(indexType, buffer::Ebo::UsageType::Dynamic);
    }
    ebo_->SetData(indices, indexSize);
  }
  uint index = 0;
  for (const AttributePointer* pointer : GetAttribPointer()) {
    pointer->Set(index++);
  }
  Unbind();
}

void Vao::Unload() {
  if (this->IsCreated()) {
    if (ebo_ != nullptr) {
      delete ebo_;
      ebo_ = nullptr;
    }
    glDeleteVertexArrays(1, &this->id_);
  }
  this->id_ = 0;
}

size_t Vao::AddAttributePointer(const std::string& bufferName,
                                const AttributePointer::DataType dataType,
                                const int elementSize,
                                const GLsizei elementStride,
                                const size_t offset, const bool perInstance) {
  auto* buffer = GetBuffer(bufferName);
  if (buffer == nullptr) {
    throw std::runtime_error("buffer with name '" + bufferName + "' not found");
  }
  const auto divisor = perInstance ? 1 : 0;
  auto* vPointer = new AttributePointer(buffer, dataType, elementSize,
                                        elementStride, divisor, offset);
  if (IsCreated()) {
    Bind();
    buffer->Bind();
    vPointer->Set(attribPointer_.size());
    Unbind();
  }
  attribPointer_.push_back(vPointer);
  return offset + elementSize * AttributePointer::GetSizeOfDataType(dataType);
}

void Vao::Bind() const {
#ifdef DEBUG
  if (this->id_ == 0) {
    PLOG_DEBUG << "Id == 0";
  }
#endif
  glBindVertexArray(this->id_);
}

void Vao::Unbind() { glBindVertexArray(0); }

buffer::Ebo* Vao::GetEbo() const { return ebo_; }

buffer::Object* Vao::GetBuffer(const std::string& name) const {
  auto itr = bufferNamesToIndex_.find(name);
  if (itr == bufferNamesToIndex_.end()) {
    return nullptr;
  }
  return bufferObjects_[itr->second];
}

buffer::Object* Vao::SetBuffer(const std::string& name, buffer::Object* vbo) {
  auto index = bufferObjects_.size();
  bufferObjects_.push_back(vbo);
  bufferNamesToIndex_.insert({name, index});
  return vbo;
}

uint Vao::GetId() const { return id_; }

const std::vector<AttributePointer*>& Vao::GetAttribPointer() const {
  return attribPointer_;
}

bool Vao::IsCreated() const { return id_ > 0; }
}  // namespace soil::video::vertex
