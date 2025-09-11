#include "stage/manager.h"

#include <ranges>
#include <stdexcept>

#include "input/manager.h"
#include "stage/scene/scene.h"
#include "stage/stage.h"
#include "window.h"

namespace soil::stage {
    Manager::Manager() : currentStage_(nullptr), resources_(nullptr), window_(nullptr), inputManager_(nullptr) {}

    Manager::~Manager() {
        if (window_ != nullptr) {
            window_->RemoveListener(this);
        }
        if (inputManager_ != nullptr) {
            inputManager_->RemoveListener(this);
        }
        delete resources_;
        for (const auto& stage : stages_ | std::views::values) {
            delete stage;
        }
    }

    void Manager::SetCurrent(const std::string& name) {
        auto* stage = GetStage(name);
        if (stage == nullptr) {
            throw std::runtime_error("Stage with name " + name + " not registered");
        }
        currentStage_ = stage;
    }

    Stage* Manager::GetCurrent() const {
        return currentStage_;
    }

    void Manager::RegisterStage(const std::string& name, Stage* stage) {
        if (stages_.contains(name)) {
            throw std::runtime_error("Stage with name " + name + " already registered");
        }
        stage->resources_ = resources_;
        stages_.insert({name, stage});
    }

    Stage* Manager::RemoveStage(const std::string& name) {
        auto* stage = GetStage(name);
        if (stage == nullptr) {
            return stage;
        }
        if (currentStage_ == stage) {
            currentStage_ = nullptr;
        }
        stage->resources_ = nullptr;
        stages_.erase(name);
        return stage;
    }

    void Manager::Init(Window* window, input::Manager* inputManager, Resources* resources) {
        window_ = window;
        inputManager_ = inputManager;
        resources_ = resources;
        window_->AddListener(this);
        inputManager_->AddListener(this);
    }

    void Manager::Update() const {
        if (currentStage_ == nullptr) {
            return;
        }
        currentStage_->Update();
    }

    void Manager::Render(video::render::State& state) const {
        if (currentStage_ == nullptr) {
            return;
        }
        currentStage_->Render(state);
    }

    void Manager::Handle(const input::Event& event) {
        if (currentStage_ == nullptr) {
            return;
        }
        currentStage_->Handle(event);
    }

    void Manager::Handle(const WindowEvent& event) {
        if (currentStage_ == nullptr) {
            return;
        }
        currentStage_->Handle(event);
    }

    Resources& Manager::GetResources() const {
        return *resources_;
    }

    Stage* Manager::GetStage(const std::string& name) const {
        const auto itr = stages_.find(name);
        if (itr == stages_.end()) {
            return nullptr;
        }
        return itr->second;
    }


} // namespace soil::stage
