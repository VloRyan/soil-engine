#include "video/render/pipeline.h"

#include <plog/Log.h>
#include <utility>

#include "stage/scene/viewer/node.h"
#include "video/render/step/render.h"
#include "video/render/step/set_renderables.h"
#include "video/render/step/sort.h"

namespace soil::video::render {
    Pipeline::Pipeline(std::string name, Container* container) : name_(std::move(name)) {
        context_.RenderContainer = container;
    }

    Pipeline::~Pipeline() {
        PLOG_DEBUG << "Delete Pipeline: " << name_;
        for (const auto* step : steps_) {
            delete step;
        }
    }

    void Pipeline::Clear() {
        PLOG_DEBUG << "Clear Pipeline: " << name_;
        steps_.clear();
    }

    void Pipeline::Run(State& state) {
        context_.State = &state;
        for (auto* step : steps_) {
            step->Process(context_);
        }
    }

    void Pipeline::InsertStep(step::Base* step) {
#ifdef DEBUG
        for (auto* s : steps_) {
            if (s->GetId() == step->GetId()) {
                throw std::invalid_argument("step already exists");
            }
        }
#endif

        if (step->GetRequiredStep() != nullptr) {
            auto itr = steps_.begin();
            find(itr, step->GetRequiredStep());

            if (itr == steps_.end()) {
                throw std::runtime_error("Can't find required step");
            }
            const auto afterRequiredStep = ++itr;
            steps_.insert(afterRequiredStep, step);
        } else {
            steps_.push_back(step);
        }
    }

    void Pipeline::RemoveStep(const step::Base* step) {
        auto itr = steps_.begin();
        find(itr, step);

        if (itr == steps_.end()) {
            throw std::runtime_error("Can't find required step");
        }
        steps_.erase(itr);
        delete step;
    }

    void Pipeline::find(std::vector<step::Base*>::iterator& itr, const step::Base* step) {
        for (; itr != steps_.end(); ++itr) {
            if (step == *itr) {
                return;
            }
        }
    }

    step::Base* Pipeline::GetStep(const std::string& id) const {
        for (auto* step : steps_) {
            if (step->GetId() == id) {
                return step;
            }
        }
        return nullptr;
    }

    std::vector<step::Base*>& Pipeline::GetSteps() {
        return steps_;
    }

    void Pipeline::SetName(std::string Name) {
        name_ = std::move(Name);
    }

    std::string Pipeline::GetName() const {
        return name_;
    }

    step::Context& Pipeline::GetContext() {
        return context_;
    }

    Pipeline* Pipeline::NewForwardRenderingPipeline(Container* container, bool depthTest, const std::string& name) {
        auto* pipeline = new Pipeline(name, container);
        pipeline->InsertStep(new step::SetRenderables("set_opaque", {.Blending = false}));
        pipeline->InsertStep(new step::Sort("sort_near_far", step::Sort::Order::FrontToBack));
        pipeline->InsertStep(new step::Render("render_opaque", {.Blend = false, .DepthTest = depthTest}));
        pipeline->InsertStep(new step::SetRenderables("set_transparent", {.Blending = true}));
        pipeline->InsertStep(new step::Sort("sort_far_near", step::Sort::Order::BackToFront));
        pipeline->InsertStep(new step::Render("render_transparent", {.Blend = true, .DepthTest = depthTest}));
        return pipeline;
    }

    bool Pipeline::Empty() const {
        return steps_.empty();
    }

    void Pipeline::Print() const {
        uint counter = 0;
        PLOG_INFO << name_ << ":";
        for (auto* step : steps_) {
            ++counter;
            PLOG_INFO << "    " << counter << ": " << step->GetId();
        }
    }
} // namespace soil::video::render
