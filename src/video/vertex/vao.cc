#include "video/vertex/vao.h"

#include "GL/gl3w.h"
#include "plog/Log.h"
#include "video/buffer/vbo.h"
#include "video/mesh/data.h"

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
  glBindVertexArray(this->id_);
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
  glBindVertexArray(0);
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
    glBindVertexArray(this->id_);
    buffer->Bind();
    vPointer->Set(attribPointer_.size());
    buffer->Unbind();
    glBindVertexArray(0);
  }
  attribPointer_.push_back(vPointer);
  return offset + elementSize * AttributePointer::GetSizeOfDataType(dataType);
}

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

Vao* Vao::NewFrom(const mesh::Data& mesh) {
  auto vao = new vertex::Vao();
  auto usage = buffer::Object::UsageType::Static;
  if (mesh.GetData() == nullptr) {
    usage = buffer::Object::UsageType::Dynamic;
  }
  auto vbo = vao->SetBuffer("data", new buffer::Vbo(usage));
  vbo->SetData(mesh.GetData(), mesh.GetDataSize());
  size_t offset = 0;
  const auto vertexSize = mesh.GetVertexSize();
  for (const auto [Elements, Type] : mesh.GetVertexAttribs()) {
    offset =
        vao->AddAttributePointer("data", Type, Elements, vertexSize, offset);
  }
  vao->CreateWithEbo(mesh.GetIndices(), mesh.GetIndexType(),
                     mesh.GetIndexCount());
  return vao;
}
}  // namespace soil::video::vertex
