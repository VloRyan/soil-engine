#include "video/render/step/bind_buffer.h"
#include "GL/gl3w.h"

namespace soil::video::render::step {
    BindBuffer::BindBuffer(const std::string& id, buffer::FrameBuffer* frameBuffer, const bool clearBuffer) :
        Base(id), frameBuffer_(frameBuffer), clearBuffer_(clearBuffer) {}

    void BindBuffer::Process(Context& context) {
        context.State->SetFramebuffer(GetFrameBuffer());
        if (const buffer::FrameBuffer* framebuffer = GetFrameBuffer(); framebuffer != nullptr) {
            context.State->SetViewPort(framebuffer->GetSize());
            if (IsClearBuffer()) {
                framebuffer->Clear();
            }
        } else {
            if (IsClearBuffer()) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }
        }
    }

    buffer::FrameBuffer* BindBuffer::GetFrameBuffer() const {
        return frameBuffer_;
    }

    bool BindBuffer::IsClearBuffer() const {
        return clearBuffer_;
    }

    void BindBuffer::SetClearBuffer(const bool clearBuffer) {
        clearBuffer_ = clearBuffer;
    }
} // namespace soil::video::render::step
