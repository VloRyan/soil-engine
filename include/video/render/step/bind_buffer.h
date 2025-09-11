#ifndef SOIL_VIDEO_RENDER_STEPBIND_BUFFER_H
#define SOIL_VIDEO_RENDER_STEPBIND_BUFFER_H
#include "base.h"

namespace soil::video::render::step {
    class BindBuffer : public Base {
    public:
        explicit BindBuffer(const std::string& id, buffer::FrameBuffer* frameBuffer = nullptr,
                            bool clearBuffer = false);
        ~BindBuffer() override = default;
        void Process(Context& context) override;
        [[nodiscard]] virtual buffer::FrameBuffer* GetFrameBuffer() const;
        [[nodiscard]] virtual bool IsClearBuffer() const;
        virtual void SetClearBuffer(bool clearBuffer);

    private:
        buffer::FrameBuffer* frameBuffer_;
        bool clearBuffer_;
    };
} // namespace soil::video::render::step

#endif
