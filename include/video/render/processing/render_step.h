#ifndef SOIL_VIDEO_RENDER_PROCESSING_RENDERSTEP_H
#define SOIL_VIDEO_RENDER_PROCESSING_RENDERSTEP_H
#include <functional>
#include <list>
#include "abstract_processing.h"
#include "video/buffer/fb.h"
#include "video/render/instance/instance.h"
#include "video/shader/shader.h"

#include "video/render/state.h"

namespace soil::video::render {
    class RenderStep final : public AbstractProcessing {
    public:
        enum class Type : std::uint8_t { Scene, TextureQuad };

        explicit RenderStep(buffer::FrameBuffer *target, Type type = Type::Scene, bool clearBeforeRender = true);

        RenderStep(buffer::FrameBuffer *target, std::string name, Type type = Type::Scene,
                   bool clearBeforeRender = true);

        ~RenderStep() override;

        void Process(Context &context, const std::vector<Renderable *> &renderables) override;

        void SetTexture(texture::Texture *texture);

        [[nodiscard]] texture::Texture *GetTexture() const;

        void SetClipPlane(glm::vec4 ClipPlane);

        [[nodiscard]] glm::vec4 GetClipPlane() const;

        void SetTextureShader(shader::Shader *textureShader, std::string textureLocatorName);

        [[nodiscard]] shader::Shader *GetTextureShader() const;

        [[nodiscard]] buffer::FrameBuffer *GetTargetBuffer() const;

        [[nodiscard]] buffer::FrameBuffer *GetRenderBuffer() const;

        void SetTexture2(texture::Texture *Texture2, std::string TextureLocator2);

        void SetMultisampleBuffer(buffer::FrameBuffer *MultisampleBuffer, bool blitToTargetBuffer = true);

        void SetFilterPredicate(std::function<bool(instance::Instance *)> FilterPredicate);

    private:
        void Render(const Context &context, const std::vector<Renderable *> &renderables) const;

        void filterInstances(const std::list<std::list<instance::Instance *> > &input,
                             std::list<std::list<instance::Instance *> > &result) const;


        Type type_;
        texture::Texture *texture_;
        std::string textureLocator_;

        texture::Texture *texture2_;
        std::string textureLocator2_;

        bool clearBeforeRender_;
        bool blitToTargetBuffer_;
        // FrameBuffer_Ptr m_SourceBuffer;
        buffer::FrameBuffer *multisampleBuffer_;
        buffer::FrameBuffer *targetBuffer_;
        shader::Shader *textureShader_;
        std::function<bool(instance::Instance *)> filterPredicate_;
        glm::vec4 clipPlane_;
    };
} // namespace soil::video::render
#endif /* SOIL_VIDEO_RENDER_PROCESSING_RENDERSTEP_H */
