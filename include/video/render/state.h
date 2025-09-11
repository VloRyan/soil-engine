
#ifndef SOIL_VIDEO_RENDER_STATE_H
#define SOIL_VIDEO_RENDER_STATE_H
#include <functional>
#include <optional>
#include <video/buffer/fb.h>
#include <video/buffer/ubo.h>

namespace soil::video::render {

    enum class DepthFunc : std::int16_t {
        Never = GL_NEVER,
        Less = GL_LESS,
        Equal = GL_EQUAL,
        LessEqual = GL_LEQUAL,
        Greater = GL_GREATER,
        NotEqual = GL_NOTEQUAL,
        GreaterEqual = GL_GEQUAL,
    };

    struct StateDef {
        std::optional<bool> Blend{};
        std::optional<bool> DepthTest{};
        std::optional<bool> StencilTest{};
        std::optional<bool> ScissorTest{};
        std::optional<render::DepthFunc> DepthFunc{DepthFunc::Less};
    };

    enum class BufferBits : std::uint16_t {
        ColorBuffer = GL_COLOR_BUFFER_BIT,
        DepthBuffer = GL_DEPTH_BUFFER_BIT,
        StencilBuffer = GL_STENCIL_BUFFER_BIT,
    };

    struct BufferBitDescription {
        bool Color{false};
        bool Depth{false};
        bool Stencil{false};
    };

    struct Rect {
        glm::ivec2 LowerLeftPosition{};
        glm::ivec2 Size{};

        friend bool operator==(const Rect& lhs, const Rect& rhs) {
            return lhs.LowerLeftPosition == rhs.LowerLeftPosition && lhs.Size == rhs.Size;
        }

        friend bool operator!=(const Rect& lhs, const Rect& rhs) {
            return !(lhs == rhs);
        }
    };

    class State final {
    public:
        State();
        virtual ~State() = default;
        void Init();

        void SetDepthTest(bool depthTest);
        [[nodiscard]] bool GetDepthTest() const;
        void SetDepthFunc(DepthFunc depthFunc);
        [[nodiscard]] DepthFunc GetDepthFunc() const;

        [[nodiscard]] bool GetBlend() const;
        void SetBlend(bool blend);

        [[nodiscard]] bool IsStencilTest() const;
        void SetStencilTest(bool stencilTest);

        [[nodiscard]] bool IsScissorTest() const;
        void SetScissorTest(bool scissorTest);
        void SetScissor(const Rect& rect);
        Rect GetScissor() const;

        [[nodiscard]] buffer::FrameBuffer* GetFramebuffer() const;
        void SetFramebuffer(buffer::FrameBuffer* framebuffer);

        [[nodiscard]] Rect GetViewPort() const;
        void SetViewPort(const Rect& rect);

        void Apply(const StateDef& def);

        void WriteUbo(int target, const std::function<void(buffer::Cursor* cursor)>& writeFunc);
#ifdef DEBUG
        void ResetChangeCounter();
#endif
        void RegisterUbo(int target, buffer::UniformBufferObject* ubo);

        [[nodiscard]] virtual int GetChanges() const;

        /**
         * Sets the texture to the next free texture unit. Default texture unit 0 is left free for loading textures.
         * @param texture to set
         * @return OpenGL texture slot (1 ... GL_MAX_TEXTURE_IMAGE_UNITS)
         */
        char SetTexture(texture::Texture& texture);
        void SetTexture(byte textureUnit, texture::Texture& texture);
        void SetTexture(GLenum target, byte textureUnit, texture::Texture& texture);

        [[nodiscard]] virtual int GetMaxImageUnits() const;

        void Clear(BufferBitDescription bits);

    private:
        bool depthTest_;
        DepthFunc depthFunc_;
        bool stencilTest_;
        bool scissorTest_;
        bool blend_;
        int changes_;
        int maxImageUnits_;

        buffer::UniformBufferObject* uboMatrices_;
        std::unordered_map<int, buffer::UniformBufferObject*> uboMap_;
        buffer::FrameBuffer* framebuffer_;
        Rect viewPort_;
        Rect scissor_;
        std::vector<texture::Texture*> textureUnits_;
    };
} // namespace soil::video::render


#endif
