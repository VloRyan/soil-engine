#include "video/buffer/vbo.h"
#include <GL/gl3w.h>
#include <plog/Log.h>

namespace video::buffer {
Vbo::Vbo(const UsageType usage, const AccessType access)
    : Object(Types::Vertex, usage, access),
      directWrite_(usage != UsageType::Static) {}

Vbo::~Vbo() {
  if (directWrite_) {
    Bind();
    if (!glUnmapBuffer(getGLBufferType(getBufferType()))) {
      PLOG_ERROR << "Data corrupted!";
    }
    cursor_ = nullptr;
  }
}

void Vbo::Reserve(const gl_size_t newBufferSize) {
  if (!directWrite_) {
    Object::Reserve(newBufferSize);
    return;
  }
  if (newBufferSize <= bufferSize_) {
    return;
  }
  this->Bind();
  if (bufferSize_ > 0) {
    PLOG_DEBUG << "BufferSize will be increased";
    if (!glUnmapBuffer(getGLBufferType(getBufferType()))) {
      PLOG_ERROR << "Data corrupted!";
    }
    cursor_ = nullptr;
  }
  GLbitfield flags =
      GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
  glBufferStorage(getGLBufferType(getBufferType()), newBufferSize, nullptr,
                  flags);
  // Map the buffer forever
  data_ = (byte *)glMapBufferRange(getGLBufferType(getBufferType()), 0,
                                   newBufferSize, flags);
  bufferSize_ = newBufferSize;
}

void Vbo::Flush() {
  if (!directWrite_) {
    Object::Flush();
    return;
  }
  if (cursor_ != nullptr) {
    // lockBuffer();
    cursor_->MoveTo(0);
  }
}

/*
void Vbo::lockBuffer() {
  if (m_SyncGPU) {
    glDeleteSync(m_SyncGPU);
  }
  //m_SyncGPU = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

void Vbo::waitBuffer() {
  while (m_SyncGPU) { // if (m_SyncGPU) - infinte loop if m_SyncGPU exsists
    GLenum waitReturn = glClientWaitSync(m_SyncGPU, GL_SYNC_FLUSH_COMMANDS_BIT,
1); if (waitReturn == GL_ALREADY_SIGNALED || waitReturn ==
GL_CONDITION_SATISFIED) { break;
    }
  }
}*/
} // namespace video::buffer
