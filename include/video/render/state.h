
#ifndef VIDEO_RENDER_STATE_H
#define VIDEO_RENDER_STATE_H
#include <functional>
#include <string>
#include <video/buffer/frame_buffer.h>
#include <video/buffer/uniform_buffer_object.h>

#include "renderable.h"
#include "window.h"

namespace video::render {
    struct StateDef {
        bool Blend{false};
        bool DepthTest{false};
        bool StencilTest{false};
    };

    class State {
    public:
        State();

        ~State() = default;

        void Init();

        [[nodiscard]] bool GetBlend() const;

        [[nodiscard]] bool GetDepthTest() const;

        void SetBlend(bool blend);

        void SetDepthTest(bool depthTest);

        [[nodiscard]] bool IsStencilTest() const;

        void SetStencilTest(bool stencilTest);

        void Apply(const StateDef &def);

        void WriteUbo(int target, const std::function<void(buffer::Cursor *cursor)> &writeFunc);
#ifdef DEBUG
        void ResetChangeCounter();
#endif
        void RegisterUbo(int target, buffer::UniformBufferObject *ubo);

        // buffer::UniformBufferObject* GetUbo(const std::string &name);
        [[nodiscard]] buffer::FrameBuffer* GetFramebuffer() const;

        void SetFramebuffer(buffer::FrameBuffer *framebuffer);

        [[nodiscard]] glm::vec2 GetViewPort() const;

        void SetViewPort(const glm::vec2 &viewPort);

    private:
        bool blend_;
        bool depthTest_;
        bool stencilTest_;
        int changes_;
        buffer::UniformBufferObject *uboMatrices_;
        std::unordered_map<int, buffer::UniformBufferObject *> uboMap_;
        buffer::FrameBuffer *framebuffer_;
        glm::vec2 viewPort_;
    };
}


#endif //VIDEO_RENDER_STATE_H
