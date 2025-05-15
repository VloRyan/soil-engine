#include "video/render/pipeline.h"
#include <plog/Log.h>

#include <utility>

#include "engine.h"
#include "stage/scene/viewer/node.h"

namespace soil::video::render {
    Pipeline::Pipeline(std::string name, Container *container) :
        name_(std::move(name)), renderToScreen_(true), container_(container),
        multisampleSceneTextureFrameBuffer_(nullptr), sceneTextureFrameBuffer_(nullptr), outputBuffer_(nullptr) {}

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
    }

    void Pipeline::Run(State &state) {
        context_.State = &state;
        context_.Container = container_;
        for (step::Base *step : processingSteps_) {
            step->Process(context_);
        }
    }

    void Pipeline::InsertStep(step::Base *step) {
        if (step->GetRequiredStep() != nullptr) {
            auto itr = processingSteps_.begin();
            find(itr, step->GetRequiredStep());

            if (itr == processingSteps_.end()) {
                throw std::runtime_error("Can't find required step");
            }
            const auto afterRequiredStep = ++itr;
            processingSteps_.insert(afterRequiredStep, step);
        } else {
            processingSteps_.push_back(step);
        }
    }

    void Pipeline::RemoveStep(const step::Base *step) {
        auto itr = processingSteps_.begin();
        find(itr, step);

        if (itr == processingSteps_.end()) {
            throw std::runtime_error("Can't find required step");
        }
        processingSteps_.erase(itr);
        delete step;
    }

    void Pipeline::find(std::vector<step::Base *>::iterator &itr, const step::Base *step) {
        for (; itr != processingSteps_.end(); ++itr) {
            if (step == *itr) {
                return;
            }
        }
    }

    step::Base *Pipeline::GetStep(const uint number) {
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

    step::Base *Pipeline::GetStep(const std::string &name) const {
        for (auto *step : processingSteps_) {
            if (step->GetId() == name) {
                return step;
            }
        }
        return nullptr;
    }

    buffer::FrameBuffer *Pipeline::GetOutputBuffer() const {
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

    step::Context &Pipeline::GetContext() {
        return context_;
    }

    bool Pipeline::Empty() const {
        return processingSteps_.empty();
    }

    void Pipeline::Print() const {
        uint counter = 0;
        PLOG_INFO << name_ << ":";
        for (auto *step : processingSteps_) {
            ++counter;
            PLOG_INFO << "    " << counter << ": " << step->GetId();
        }
    }
} // namespace soil::video::render
