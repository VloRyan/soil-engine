#include "stage/manager.h"

#include "input/manager.h"
#include "stage/stage.h"
#include "window.h"

#include "stage/scene/scene.h"

namespace soil::stage {
    Manager::Manager() :
        currentStage_(-1), window_(nullptr), inputManager_(nullptr), resources_(nullptr), nextStageId_(0) {}

    Manager::~Manager() {
        if (window_ != nullptr) {
            window_->RemoveListener(this);
        }
        if (inputManager_ != nullptr) {
            inputManager_->RemoveListener(this);
        }
        for (const auto *stage : stages_) {
            delete stage;
        }
    }

    int Manager::AddStage(Stage *stage) {
        stages_.emplace_back(stage);
        if (stages_.size() == 1) {
            currentStage_ = 0;
        }
        return ++nextStageId_;
    }

    void Manager::Init(Window *window, input::Manager *inputManager, Resources *resources) {
        window_ = window;
        inputManager_ = inputManager;
        resources_ = resources;
        window_->AddListener(this);
        inputManager_->AddListener(this);
    }

    void Manager::Update() const {
#ifdef DEBUG
        if (currentStage_ == -1) {
            throw std::runtime_error("currentStage_ not set");
        }
#endif
        stages_[currentStage_]->Update();
    }

    void Manager::Render(video::render::State &state) const {
#ifdef DEBUG
        if (currentStage_ == -1) {
            throw std::runtime_error("currentStage_ not set");
        }
#endif
        stages_[currentStage_]->Render(state);
    }

    void Manager::Handle(const input::Event &event) {
#ifdef DEBUG
        if (currentStage_ == -1) {
            throw std::runtime_error("currentStage_ not set");
        }
#endif
        stages_[currentStage_]->Handle(event);
    }

    void Manager::Handle(const WindowEvent &event) {
#ifdef DEBUG
        if (currentStage_ == -1) {
            throw std::runtime_error("currentStage_ not set");
        }
#endif
        stages_[currentStage_]->Handle(event);
    }

    Resources &Manager::GetResources() const { return *resources_; }


} // namespace soil::stage
