
#ifndef SOIL_VIDEO_RENDER_STATE_H
#define SOIL_VIDEO_RENDER_STATE_H
#include <functional>
#include <optional>
#include <video/buffer/fb.h>
#include <video/buffer/ubo.h>

namespace soil::video::render {
    struct StateDef {
        std::optional<bool> Blend{};
        std::optional<bool> DepthTest{};
        std::optional<bool> StencilTest{};
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

        [[nodiscard]] buffer::FrameBuffer *GetFramebuffer() const;

        void SetFramebuffer(buffer::FrameBuffer *framebuffer);

        [[nodiscard]] glm::ivec2 GetViewPort() const;

        void SetViewPort(const glm::ivec2 &viewPort);

    private:
        bool blend_;
        bool depthTest_;
        bool stencilTest_;
        int changes_;
        buffer::UniformBufferObject *uboMatrices_;
        std::unordered_map<int, buffer::UniformBufferObject *> uboMap_;
        buffer::FrameBuffer *framebuffer_;
        glm::ivec2 viewPort_;
    };
} // namespace soil::video::render


#endif // SOIL_VIDEO_RENDER_STATE_H
