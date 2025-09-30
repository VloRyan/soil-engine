#include "video/buffer/object.h"

#include <plog/Log.h>

#include "GL/gl3w.h"

namespace soil::video::buffer {
Object::Object(const Types bufferType, const UsageType usage,
               const AccessType access)
    : data_(nullptr),
      cursor_(nullptr),
      id_(0),
      bufferSize_(0),
      bufferType_(bufferType),
      usage_(usage),
      access_(access) {}

Object::~Object() {
  if (!IsCreated()) {
    return;
  }

  PLOG_DEBUG << "Unload Object " << std::to_string(id_) << " with type "
             << std::to_string(static_cast<int>(bufferType_));
  glDeleteBuffers(1, &id_);
  delete cursor_;
  delete[] data_;
  id_ = 0;
  bufferSize_ = 0;
}

void Object::create() {
  if (IsCreated()) {
    PLOG_DEBUG << "Object " << std::to_string(id_) << "already created";
  }
  glGenBuffers(1, &id_);
}

Cursor *Object::GetCursor() {
  if (cursor_ == nullptr) {
    cursor_ = new Cursor(data_);
  }
  return cursor_;
}

void Object::Reserve(const gl_size_t newBufferSize) {
  if (newBufferSize <= GetBufferSize()) {
    return;
  }
#ifdef DEBUG
  if (GetBufferSize() > 0) {
    PLOG_DEBUG << "BufferSize will be increased";
  }
#endif
  this->Bind();
  glBufferData(getGLBufferType(getBufferType()), newBufferSize, nullptr,
               getGLUsageType());
  if (data_ != nullptr) {
    glBufferSubData(getGLBufferType(getBufferType()), 0, bufferSize_, data_);
    auto *newBuffer = new byte[newBufferSize];
    memcpy(newBuffer, data_, bufferSize_);
    delete[] data_;
    data_ = newBuffer;
  } else {
    delete[] data_;
    data_ = new byte[newBufferSize];
  }
  bufferSize_ = newBufferSize;
}

void Object::SetData(const void *data, const gl_size_t dataSize) {
  Reserve(dataSize);
  if (data != nullptr) {
    auto *cursor = GetCursor();
    cursor->MoveTo(0);
    cursor->Write(data, dataSize);
    Flush();
  }
}

void Object::Flush() {
  if (cursor_ != nullptr) {
    uploadData(GetData(), static_cast<int>(cursor_->GetDataWritten()));
    cursor_->MoveTo(0);
  }
}

void Object::uploadData(const void *data, const gl_size_t dataSize) {
  if (dataSize > GetBufferSize()) {
    Reserve(dataSize);  // also sets the current data
  } else {
    this->Bind();
    // Buffer orphaning, a common way to improve streaming perf.
    glBufferData(getGLBufferType(getBufferType()), GetBufferSize(), nullptr,
                 getGLUsageType());
    glBufferSubData(getGLBufferType(getBufferType()), 0, dataSize, data);
  }
}

GLenum Object::getGLUsageType() const {
  switch (this->GetAccess()) {
    case AccessType::Draw:
      switch (this->GetUsage()) {
        case UsageType::Dynamic:
          return GL_DYNAMIC_DRAW;
        case UsageType::Stream:
          return GL_STREAM_DRAW;
        case UsageType::Static:
          return GL_STATIC_DRAW;
        default:
          throw std::runtime_error(
              "unknown usage type: " +
              std::to_string(static_cast<uint>(this->GetUsage())));
      }
    case AccessType::Read:
      switch (this->GetUsage()) {
        case UsageType::Dynamic:
          return GL_DYNAMIC_READ;
        case UsageType::Stream:
          return GL_STREAM_READ;
        case UsageType::Static:
          return GL_STATIC_READ;
        default:
          throw std::runtime_error(
              "unknown usage type: " +
              std::to_string(static_cast<uint>(this->GetUsage())));
      }
    case AccessType::Copy:
      switch (this->GetUsage()) {
        case UsageType::Dynamic:
          return GL_DYNAMIC_COPY;
        case UsageType::Stream:
          return GL_STREAM_COPY;
        case UsageType::Static:
          return GL_STATIC_COPY;
        default:
          throw std::runtime_error(
              "unknown usage type: " +
              std::to_string(static_cast<uint>(this->GetUsage())));
      }
    default:
      throw std::runtime_error(
          "unknown access type: " +
          std::to_string(static_cast<uint>(this->GetAccess())));
  }
}

GLenum Object::getGLBufferType(const Types type) const {
  switch (type) {
    case Types::Vertex:
      return GL_ARRAY_BUFFER;
    case Types::ElementArray:
      return GL_ELEMENT_ARRAY_BUFFER;
    case Types::Uniform:
      return GL_UNIFORM_BUFFER;
    case Types::ShaderStorage:
      return GL_SHADER_STORAGE_BUFFER;
    case Types::DrawIndirect:
      return GL_DRAW_INDIRECT_BUFFER;
  }
  throw std::runtime_error(
      "Unknown buffer type: " +
      std::to_string(static_cast<uint>(this->getBufferType())));
}

void Object::Bind() const { BindAs(getBufferType()); }

void Object::Unbind() const { UnbindAs(getBufferType()); }

void Object::BindAs(const Types type) const {
#ifdef DEBUG
  if (!IsCreated()) {
    throw std::runtime_error(
        "Can not bind buffer object because it is not created!");
  }
#endif
  const GLenum glType = getGLBufferType(type);
  glBindBuffer(glType, GetId());
}

void Object::UnbindAs(const Types type) const {
  const GLenum glType = getGLBufferType(type);
  glBindBuffer(glType, 0);
}

Object::AccessType Object::GetAccess() const { return access_; }

uint Object::GetBufferSize() const { return bufferSize_; }

Object::Types Object::getBufferType() const { return bufferType_; }

const void *Object::GetData() const { return data_; }

uint Object::GetId() const { return id_; }

Object::UsageType Object::GetUsage() const { return usage_; }

bool Object::IsUsage(const UsageType desired) const {
  return usage_ == desired;
}

bool Object::IsCreated() const { return id_ != 0; }

void Object::BindToTarget(const uint index) const {
  BindToTarget(index, getBufferType());
}

void Object::BindToTarget(const uint index, const Types target) const {
#ifdef DEBUG
  if (!IsCreated()) {
    throw std::runtime_error("bindToTarget: BufferObject is not created!");
  }
#endif
  switch (target) {
    case Types::ShaderStorage:
    case Types::Uniform:
      break;
    default:
      throw std::runtime_error("Invalid target for binding: " +
                               std::to_string(static_cast<uint>(target)));
  }
  const GLenum glTarget = getGLBufferType(target);
  glBindBufferBase(glTarget, index, GetId());
}
}  // namespace soil::video::buffer
