#include "stage/manager.h"

#include <ranges>
#include <stdexcept>

#include "input/manager.h"
#include "stage/scene/scene.h"
#include "stage/stage.h"

namespace soil::stage {
    Manager::Manager(Resources* resources) :
        currentStage_(nullptr), nextStage_(nullptr), resources_(resources) {
    }

    Manager::~Manager() {
        for (const auto& stage : stages_ | std::views::values) {
            delete stage;
        }
    }

    void Manager::SetCurrent(const std::string& name) {
        nextStage_ = GetStage(name);
        if (nextStage_ == nullptr) {
            throw std::runtime_error("Stage with name " + name + " not registered");
        }
        if (!nextStage_->IsLoaded()) {
            nextStage_->Load();
        }
        if (currentStage_ == nullptr) {
            currentStage_ = nextStage_;
        }
    }

    void Manager::SetCurrent(Stage* stage) {
        nextStage_ = stage;
#ifdef DEBUG
        auto known = false;
        for (const auto* regStage : stages_ | std::views::values) {
            if (stage == regStage) {
                known = true;
            }
        }
        if (!known) {
            throw std::runtime_error("Stage is unknown");
        }
#endif
        if (!nextStage_->IsLoaded()) {
            nextStage_->Load();
        }
    }

    Stage* Manager::GetCurrent() const {
        return currentStage_;
    }

    void Manager::RegisterStage(const std::string& name, Stage* stage) {
        if (stages_.contains(name)) {
            throw std::runtime_error("Stage with name " + name + " already registered");
        }
        stage->resources_ = resources_;
        stage->manager_ = this;
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

    void Manager::Update() {
        if (nextStage_ != nullptr) {
            auto* prevStage = currentStage_;
            currentStage_ = nextStage_;
            nextStage_ = nullptr;
            const auto stageChangedEvent = event::StageEvent::MakeActiveStageChanged(currentStage_, prevStage);
            for (auto* stage : stages_ | std::views::values) {
                stage->Handle(stageChangedEvent);
            }
        }
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
