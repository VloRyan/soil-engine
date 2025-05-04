#include "video/render/processing/render_step.h"
#include "GL/gl3w.h"
#include "exception.h"

namespace video::render {
    RenderStep::RenderStep(buffer::FrameBuffer *target, const Type type, const bool clearBeforeRender)
        : type_(type),
          texture_(nullptr),
          texture2_(nullptr),
          clearBeforeRender_(clearBeforeRender),
          blitToTargetBuffer_(false),
          multisampleBuffer_(nullptr),
          targetBuffer_(target),
          textureShader_(nullptr),
          filterPredicate_(nullptr),
          clipPlane_(0) {
    }

    RenderStep::RenderStep(buffer::FrameBuffer *target, std::string name, const Type type, const bool clearBeforeRender)
        : AbstractProcessing(std::move(name)),
          type_(type),
          texture_(nullptr),
          texture2_(nullptr),
          clearBeforeRender_(clearBeforeRender),
          blitToTargetBuffer_(false),
          multisampleBuffer_(nullptr),
          targetBuffer_(target),
          textureShader_(nullptr),
          filterPredicate_(nullptr),
          clipPlane_(0) {
    }

    RenderStep::~RenderStep() {
        targetBuffer_ = nullptr;
        multisampleBuffer_ = nullptr;
        texture_ = nullptr;
    }

    void RenderStep::Process(Context &context, const std::vector<Renderable *> &renderables) {
        //START_MEASURE_TIME("New renderbuffer")
        if (const buffer::FrameBuffer *renderBuffer = getRenderBuffer(); renderBuffer != nullptr) {
            renderBuffer->Bind();
            if (clearBeforeRender_) {
                renderBuffer->Clear();
            }
            glViewport(0, 0, renderBuffer->GetSize().x, renderBuffer->GetSize().y);
        }

        //STOP_MEASURE_TIME("New renderbuffer")
        //START_MEASURE_TIME("Clear renderbuffer")

        //STOP_MEASURE_TIME("Clear renderbuffer")
        /* switch (type_) {
             case Type::Scene:
                 //  START_MEASURE_TIME("Render renderPileVector")*/
        Render(context, renderables);
        //STOP_MEASURE_TIME("Render renderPileVector")
        /*break;
    case Type::TextureQuad:
        renderTexturedQuad(context);
        break;
}*/
        //  START_MEASURE_TIME("multisampleBuffer_ blitTo")
        if (blitToTargetBuffer_ && multisampleBuffer_ != nullptr && targetBuffer_ != nullptr) {
            multisampleBuffer_->BlitTo(*targetBuffer_);
        }
        //STOP_MEASURE_TIME("multisampleBuffer_ blitTo")
    }

    void RenderStep::Render(const Context &context, const std::vector<Renderable *> &renderables) const {
        // context->GetProperties()->SetClippingPlane(0, clipPlane_);
        context.GetTechnique()->Render(context.GetState(), renderables);
    }

    void RenderStep::filterInstances(const std::list<std::list<instance::Instance *> > &input,
                                     std::list<std::list<instance::Instance *> > &result) const {
        for (const std::list<instance::Instance *> &list: input) {
            std::list<instance::Instance *> tmpList;
            for (instance::Instance *instance: list) {
                if (!filterPredicate_(instance)) {
                    tmpList.push_back(instance);
                }
            }
            if (!tmpList.empty()) {
                result.push_back(tmpList);
            }
        }
    }

    /*  void RenderStep::renderTexturedQuad(const Context *context) const {
          if (textureShader_ == nullptr) {
              throw engine::Exception("No TextureShader defined.");
          }
          const auto *const quad = context->GetQuad();
          textureShader_->Use();
          textureShader_->SetTexture2d(0, getTexture(), textureLocator_);
          if (texture2_ != nullptr) {
              textureShader_->SetTexture2d(1, texture2_, textureLocator2_);
          }
          quad->GetVao()->Bind();
          if (context->GetProperties()->isWireFrame()) {
              glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          }

          //glEnable(GL_BLEND);
          //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          video::shader::Shader::drawElements(GL_TRIANGLES, quad->GetIndexCount(), quad->GetIndexType());
          //glDisable(GL_BLEND);
          video::vertex::Vao::Unbind();
          textureShader_->leave();
          if (context->GetProperties()->isWireFrame()) {
              /*Wireframe*/
    /*          glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
          }
      }
  */
    texture::Texture* RenderStep::getTexture() const {
        return texture_;
    }

    void RenderStep::setTexture(texture::Texture *texture) {
        texture_ = texture;
    }

    glm::vec4 RenderStep::getClipPlane() const {
        return clipPlane_;
    }

    void RenderStep::setClipPlane(const glm::vec4 ClipPlane) {
        clipPlane_ = ClipPlane;
    }

    void RenderStep::setTextureShader(shader::Shader *textureShader, std::string textureLocatorName) {
        textureShader_ = textureShader;
        textureLocator_ = std::move(textureLocatorName);
    }

    shader::Shader* RenderStep::getTextureShader() const {
        return textureShader_;
    }

    buffer::FrameBuffer* RenderStep::getTargetBuffer() const {
        return targetBuffer_;
    }

    void RenderStep::setTexture2(texture::Texture *Texture2, std::string TextureLocator2) {
        texture2_ = Texture2;
        textureLocator2_ = std::move(TextureLocator2);
    }

    void RenderStep::setMultisampleBuffer(buffer::FrameBuffer *MultisampleBuffer, bool blitToTargetBuffer) {
        multisampleBuffer_ = MultisampleBuffer;
        blitToTargetBuffer_ = blitToTargetBuffer;
    }

    void RenderStep::setFilterPredicate(std::function<bool(instance::Instance *)> FilterPredicate) {
        filterPredicate_ = std::move(FilterPredicate);
    }

    buffer::FrameBuffer* RenderStep::getRenderBuffer() const {
        if (multisampleBuffer_ != nullptr) {
            return multisampleBuffer_;
        }
        return targetBuffer_;
    }
}
