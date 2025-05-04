#ifndef VIDEO_RENDER_PROCESSING_RENDERSTEP_H
#define VIDEO_RENDER_PROCESSING_RENDERSTEP_H
#include "abstract_processing.h"
#include "video/buffer/frame_buffer.h"
#include "video/render/instance/instance.h"
#include "video/shader/shader.h"
#include <functional>
#include <list>

#include "video/render/state.h"

namespace video::render {
    class RenderStep : public AbstractProcessing {
    public:
        enum class Type : std::uint8_t {
            Scene,
            TextureQuad
        };

        explicit RenderStep(buffer::FrameBuffer *target, Type type = Type::Scene, bool clearBeforeRender = true);

        RenderStep(buffer::FrameBuffer *target, std::string name, Type type = Type::Scene,
                   bool clearBeforeRender = true);

        ~RenderStep() override;

        void Process(Context &context, const std::vector<Renderable *> &renderables) override;

        void setTexture(texture::Texture *texture);

        [[nodiscard]] texture::Texture* getTexture() const;

        void setClipPlane(glm::vec4 ClipPlane);

        [[nodiscard]] glm::vec4 getClipPlane() const;

        void setTextureShader(shader::Shader *textureShader, std::string textureLocatorName);

        [[nodiscard]] shader::Shader* getTextureShader() const;

        [[nodiscard]] buffer::FrameBuffer* getTargetBuffer() const;

        [[nodiscard]] buffer::FrameBuffer* getRenderBuffer() const;

        void setTexture2(texture::Texture *Texture2, std::string TextureLocator2);

        void setMultisampleBuffer(buffer::FrameBuffer *MultisampleBuffer, bool blitToTargetBuffer = true);

        void setFilterPredicate(std::function<bool(instance::Instance *)> FilterPredicate);

    private:
        void Render(const Context &context, const std::vector<Renderable *> &renderables) const;

        void renderTexturedQuad(const Context *context) const;

        void filterInstances(const std::list<std::list<instance::Instance *> > &input,
                             std::list<std::list<instance::Instance *> > &result) const;


        Type type_;
        texture::Texture *texture_;
        std::string textureLocator_;

        texture::Texture *texture2_;
        std::string textureLocator2_;

        bool clearBeforeRender_;
        bool blitToTargetBuffer_;
        //FrameBuffer_Ptr m_SourceBuffer;
        buffer::FrameBuffer *multisampleBuffer_;
        buffer::FrameBuffer *targetBuffer_;
        shader::Shader *textureShader_;
        std::function<bool(instance::Instance *)> filterPredicate_;
        glm::vec4 clipPlane_;
    };
}
#endif /* VIDEO_RENDER_PROCESSING_RENDERSTEP_H */
