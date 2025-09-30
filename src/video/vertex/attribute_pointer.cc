#include "video/vertex/attribute_pointer.h"

#include <stdexcept>
#include <string>

#include "GL/gl3w.h"

namespace soil::video::vertex {
AttributePointer::AttributePointer(buffer::Object* vbo, const DataType dataType,
                                   const int elementSize,
                                   const GLsizei elementStride,
                                   const byte divisor, const size_t offset)
    : vbo_(vbo),
      dataType_(dataType),
      elementSize_(elementSize),
      elementStride_(elementStride),
      divisor_(divisor),
      offset_(offset),
      normalize_(false) {}

void AttributePointer::Set(const uint index) const {
  vbo_->BindAs(buffer::Object::Types::Vertex);
  const auto glType = static_cast<GLenum>(dataType_);
  glEnableVertexAttribArray(index);

  const bool isIntegerType = glType == GL_BYTE || glType == GL_UNSIGNED_BYTE ||
                             glType == GL_INT || glType == GL_UNSIGNED_INT;

  if (isIntegerType) {
    if (!normalize_) {
      glVertexAttribIPointer(index, this->GetElementSize(), glType,
                             this->GetElementStride(),
                             reinterpret_cast<void*>(this->GetOffset()));
    } else {
      glVertexAttribPointer(index, this->GetElementSize(), glType, GL_TRUE,
                            this->GetElementStride(),
                            reinterpret_cast<void*>(this->GetOffset()));
    }
  } else {
    const auto normalize = normalize_ ? GL_TRUE : GL_FALSE;
    glVertexAttribPointer(index, this->GetElementSize(), glType, normalize,
                          this->GetElementStride(),
                          reinterpret_cast<void*>(this->GetOffset()));
  }
  if (this->GetDivisor() != 0U) {
    glVertexAttribDivisor(index, this->GetDivisor());
  }
  vbo_->UnbindAs(buffer::Object::Types::Vertex);
}

GLsizei AttributePointer::GetSizeOfDataType(const DataType type) {
  switch (type) {
    case Double:
    case UInt64:
      return 8;  // 64 bit
    case Float:
    case Int:
    case UInt:
      return 4;  // 32 bit
    case Byte:
    case UByte:
      return 1;  // 8 bit
    default:
      throw std::runtime_error("Unknown data type: " +
                               std::to_string(static_cast<int>(type)));
  }
}

AttributePointer::DataType AttributePointer::GetDataType() const {
  return dataType_;
}

byte AttributePointer::GetDivisor() const { return divisor_; }

int AttributePointer::GetElementSize() const { return elementSize_; }

GLsizei AttributePointer::GetElementStride() const { return elementStride_; }

size_t AttributePointer::GetOffset() const { return offset_; }

bool AttributePointer::IsPerInstance() const { return divisor_ != 0U; }

void AttributePointer::SetDataType(const DataType DataType) {
  dataType_ = DataType;
}

buffer::Object* AttributePointer::GetVbo() const { return vbo_; }

void AttributePointer::SetNormalize(const bool Normalize) {
  normalize_ = Normalize;
}

bool AttributePointer::IsNormalize() const { return normalize_; }
}  // namespace soil::video::vertex
