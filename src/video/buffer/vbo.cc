#include "video/buffer/vbo.h"
#include <GL/gl3w.h>
#include <plog/Log.h>

namespace soil::video::buffer {
    Vbo::Vbo(const UsageType usage, const AccessType access) :
        Object(Types::Vertex, usage, access), directWrite_(usage != UsageType::Static) {}

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
        constexpr GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glBufferStorage(getGLBufferType(getBufferType()), newBufferSize, nullptr, flags);
        // Map the buffer forever
        data_ = static_cast<byte *>(glMapBufferRange(getGLBufferType(getBufferType()), 0, newBufferSize, flags));
        bufferSize_ = newBufferSize;
    }

    void Vbo::Flush() {
        if (!directWrite_) {
            Object::Flush();
            return;
        }
        if (cursor_ != nullptr) {
            cursor_->MoveTo(0);
        }
    }
} // namespace soil::video::buffer
