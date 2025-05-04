#include "pipeline.h"
#include <plog/Log.h>

#include <utility>

#include "stage/scene/viewer/node.h"
#include "engine.h"

namespace video::render {
    Pipeline::Pipeline(std::string name)
        : name_(std::move(name)),
          renderToScreen_(true),
          renderSceneToTextureStep_(nullptr),
          multisampleSceneTextureFrameBuffer_(nullptr),
          sceneTextureFrameBuffer_(nullptr),
          outputBuffer_(nullptr) {
        context_.SetProperties(new Properties());
        SetRenderingTechnique(TechniqueType::Forward);
    }

    Pipeline::~Pipeline() {
        PLOG_DEBUG << "Delete Pipeline: " << name_;
        processingSteps_.clear();
    }

    void Pipeline::Clear() {
        PLOG_DEBUG << "Clear Pipeline: " << name_;
        processingSteps_.clear();
        multisampleSceneTextureFrameBuffer_ = nullptr;
        sceneTextureFrameBuffer_ = nullptr;
        outputBuffer_ = nullptr;
        renderSceneToTextureStep_ = nullptr;
    }

    void Pipeline::Reset(const bool multisample, const glm::ivec2 resolution) {
        PLOG_DEBUG << "Reset Pipeline: " << name_ << ", multisample: " << std::to_string(multisample);
        processingSteps_.clear();
        if (sceneTextureFrameBuffer_ == nullptr) {
            //sceneTextureFrameBuffer_ = new buffer::FrameBuffer(resolution);
            //sceneTextureFrameBuffer_->createColorTexture();
            //sceneTextureFrameBuffer_->createDepthBuffer();
            //outputBuffer_ = sceneTextureFrameBuffer_;
            //sceneTextureFrameBuffer_ = buffer::FrameBuffer::Screen();
        } else {
            // outputBuffer_->Resize(resolution);
        }
        delete renderSceneToTextureStep_;
        //renderSceneToTextureStep_ = new RenderStep(sceneTextureFrameBuffer_, "RenderSceneToTexture");
        renderSceneToTextureStep_ = new RenderStep(sceneTextureFrameBuffer_, "Render");
        /*if (multisample && (multisampleSceneTextureFrameBuffer_ == nullptr)) {
            multisampleSceneTextureFrameBuffer_ = new buffer::FrameBuffer(resolution);
            multisampleSceneTextureFrameBuffer_->createColorBuffer(4);
            multisampleSceneTextureFrameBuffer_->createDepthBuffer(buffer::FrameBuffer::DepthBufferType::Depth, 4);
            renderSceneToTextureStep_->setMultisampleBuffer(multisampleSceneTextureFrameBuffer_);
        }*/

        InsertStep(renderSceneToTextureStep_, nullptr);
    }

    void Pipeline::Render(const std::vector<Renderable *> &renderables, State &state) {
        context_.SetState(state);
        for (AbstractProcessing *step: processingSteps_) {
            //START_MEASURE_TIME(name_ + "-> " + step->GetName());
            step->Process(context_, renderables);
            //STOP_MEASURE_TIME(name_ + "-> " + step->GetName());
        }
    }

    Technique* Pipeline::GetRenderingTechnique() const {
        return context_.GetTechnique();
    }

    void Pipeline::SetRenderingTechnique(const TechniqueType type) {
        auto *technique = Technique::GetTechnique(type);
        context_.SetTechnique(technique);
    }

    void Pipeline::InsertStep(AbstractProcessing *step, AbstractProcessing *requiredStep) {
        if (requiredStep != nullptr) {
            if (step == requiredStep) {
                throw engine::Exception("Self-Reference");
            }
            auto itr = processingSteps_.begin();
            find(itr, requiredStep);

            if (itr == processingSteps_.end()) {
                throw engine::Exception("Can't find required step");
            }
            const auto afterRequiredStep = ++itr;
            processingSteps_.insert(afterRequiredStep, step);
        } else {
            processingSteps_.insert(processingSteps_.begin(), step);
        }
        step->requiredStep_ = requiredStep;
        step->onAttach();
    }

    void Pipeline::RemoveStep(const AbstractProcessing *step) {
        auto itr = processingSteps_.begin();
        find(itr, step);

        if (itr == processingSteps_.end()) {
            throw engine::Exception("Can't find required step");
        }
        processingSteps_.erase(itr);
        delete step;
    }

    void Pipeline::find(std::vector<AbstractProcessing *>::iterator &itr, const AbstractProcessing *step) {
        for (; itr != processingSteps_.end(); ++itr) {
            if (step == *itr) {
                return;
            }
        }
    }

    AbstractProcessing* Pipeline::GetStep(const uint number) {
        auto itr = processingSteps_.begin();
        uint counter = 0;
        for (; itr != processingSteps_.end(); ++itr) {
            if (counter == number) {
                return *itr;
            }
            ++counter;
        }
        return nullptr;
    }

    AbstractProcessing* Pipeline::GetStep(const std::string &name) {
        //uint counter = 0;
        for (auto *step: processingSteps_) {
            if (step->GetName() == name) {
                return step;
            }
        }
        return nullptr;
    }

    /*
    FrameBuffer_Ptr Pipeline::getFrameBuffer(std::string name) {
      auto itr = m_NamedFrameBuffer.find(name);
      bool contains = itr != m_NamedFrameBuffer.end();
      if (contains) {
  
        return m_NamedFrameBuffer.at(name);
      }
      return nullptr;
    }
  
    void Pipeline::registerFrameBuffer(std::string name, FrameBuffer_Ptr buffer) {
      if (buffer) {
        std::pair<std::string, FrameBuffer_Ptr> pair(name, buffer);
        auto insertResult = m_NamedFrameBuffer.insert(pair);
        bool isAlreadyInserted = !insertResult.second;
        if (isAlreadyInserted) {
          throw Exception("Create failed");
        }
        LOGGER->debug("registerFrameBuffer %s -> %d", name.c_str(), buffer->GetId());
      } else {
  
        m_NamedFrameBuffer.erase(name);
        LOGGER->debug("deregisterFrameBuffer %s", name.c_str());
      }
    }
     */


    buffer::FrameBuffer* Pipeline::GetOutputBuffer() const {
        return outputBuffer_;
    }

    void Pipeline::SetOutputBuffer(buffer::FrameBuffer *buffer) {
        outputBuffer_ = buffer;
    }

    bool Pipeline::IsRenderToScreen() const {
        return renderToScreen_;
    }

    void Pipeline::SetRenderToScreen(const bool RenderToScreen) {
        renderToScreen_ = RenderToScreen;
    }

    void Pipeline::SetName(std::string Name) {
        name_ = std::move(Name);
    }

    std::string Pipeline::GetName() const {
        return name_;
    }

    bool Pipeline::Empty() const {
        return processingSteps_.empty();
    }

    void Pipeline::Print() {
        uint counter = 0;
        PLOG_INFO << name_ << ":";
        for (auto *step: processingSteps_) {
            ++counter;
            PLOG_INFO << "    " << counter << ": " << step->GetName();
        }
    }
} // Render
