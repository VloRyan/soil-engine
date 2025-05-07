#include "video/render/processing/render_step.h"
#include "GL/gl3w.h"

namespace soil::video::render {
    RenderStep::RenderStep(buffer::FrameBuffer *target, const Type type, const bool clearBeforeRender) :
        type_(type), texture_(nullptr), texture2_(nullptr), clearBeforeRender_(clearBeforeRender),
        blitToTargetBuffer_(false), multisampleBuffer_(nullptr), targetBuffer_(target), textureShader_(nullptr),
        filterPredicate_(nullptr), clipPlane_(0) {}

    RenderStep::RenderStep(buffer::FrameBuffer *target, std::string name, const Type type,
                           const bool clearBeforeRender) :
        AbstractProcessing(std::move(name)), type_(type), texture_(nullptr), texture2_(nullptr),
        clearBeforeRender_(clearBeforeRender), blitToTargetBuffer_(false), multisampleBuffer_(nullptr),
        targetBuffer_(target), textureShader_(nullptr), filterPredicate_(nullptr), clipPlane_(0) {}

    RenderStep::~RenderStep() {
        targetBuffer_ = nullptr;
        multisampleBuffer_ = nullptr;
        texture_ = nullptr;
    }

    void RenderStep::Process(Context &context, const std::vector<Renderable *> &renderables) {
        // START_MEASURE_TIME("New renderbuffer")
        if (const buffer::FrameBuffer *renderBuffer = GetRenderBuffer(); renderBuffer != nullptr) {
            renderBuffer->Bind();
            if (clearBeforeRender_) {
                renderBuffer->Clear();
            }
            glViewport(0, 0, renderBuffer->GetSize().x, renderBuffer->GetSize().y);
        }

        // STOP_MEASURE_TIME("New renderbuffer")
        // START_MEASURE_TIME("Clear renderbuffer")

        // STOP_MEASURE_TIME("Clear renderbuffer")
        /* switch (type_) {
             case Type::Scene:
                 //  START_MEASURE_TIME("Render renderPileVector")*/
        Render(context, renderables);
        // STOP_MEASURE_TIME("Render renderPileVector")
        /*break;
    case Type::TextureQuad:
        renderTexturedQuad(context);
        break;
}*/
        //  START_MEASURE_TIME("multisampleBuffer_ blitTo")
        if (blitToTargetBuffer_ && multisampleBuffer_ != nullptr && targetBuffer_ != nullptr) {
            multisampleBuffer_->BlitTo(*targetBuffer_);
        }
        // STOP_MEASURE_TIME("multisampleBuffer_ blitTo")
    }

    void RenderStep::Render(const Context &context, const std::vector<Renderable *> &renderables) const {
        // context->GetProperties()->SetClippingPlane(0, clipPlane_);
        context.GetTechnique()->Render(context.GetState(), renderables);
    }

    void RenderStep::filterInstances(const std::list<std::list<instance::Instance *> > &input,
                                     std::list<std::list<instance::Instance *> > &result) const {
        for (const std::list<instance::Instance *> &list : input) {
            std::list<instance::Instance *> tmpList;
            for (instance::Instance *instance : list) {
                if (!filterPredicate_(instance)) {
                    tmpList.push_back(instance);
                }
            }
            if (!tmpList.empty()) {
                result.push_back(tmpList);
            }
        }
    }

    texture::Texture *RenderStep::GetTexture() const { return texture_; }

    void RenderStep::SetTexture(texture::Texture *texture) { texture_ = texture; }

    glm::vec4 RenderStep::GetClipPlane() const { return clipPlane_; }

    void RenderStep::SetClipPlane(const glm::vec4 ClipPlane) { clipPlane_ = ClipPlane; }

    void RenderStep::SetTextureShader(shader::Shader *textureShader, std::string textureLocatorName) {
        textureShader_ = textureShader;
        textureLocator_ = std::move(textureLocatorName);
    }

    shader::Shader *RenderStep::GetTextureShader() const { return textureShader_; }

    buffer::FrameBuffer *RenderStep::GetTargetBuffer() const { return targetBuffer_; }

    void RenderStep::SetTexture2(texture::Texture *Texture2, std::string TextureLocator2) {
        texture2_ = Texture2;
        textureLocator2_ = std::move(TextureLocator2);
    }

    void RenderStep::SetMultisampleBuffer(buffer::FrameBuffer *MultisampleBuffer, const bool blitToTargetBuffer) {
        multisampleBuffer_ = MultisampleBuffer;
        blitToTargetBuffer_ = blitToTargetBuffer;
    }

    void RenderStep::SetFilterPredicate(std::function<bool(instance::Instance *)> FilterPredicate) {
        filterPredicate_ = std::move(FilterPredicate);
    }

    buffer::FrameBuffer *RenderStep::GetRenderBuffer() const {
        if (multisampleBuffer_ != nullptr) {
            return multisampleBuffer_;
        }
        return targetBuffer_;
    }
} // namespace soil::video::render
