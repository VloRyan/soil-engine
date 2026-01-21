#include "stage/manager.h"

#include <ranges>
#include <stdexcept>

#include "engine.h"
#include "input/manager.h"
#include "stage/stage.h"
namespace soil::stage {
Manager::Manager(Resources& resources)
    : currentStage_(nullptr), nextStage_(nullptr), resources_(resources) {}

Manager::~Manager() {
  for (const auto& stage : stages_ | std::views::values) {
    delete stage;
  }
}

void Manager::SetCurrent(const std::string& name) {
  nextStage_ = GetStage(name);
  if (nextStage_ == nullptr) {
    throw std::runtime_error("Stage with name " + name + " is unknown");
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
}

Stage* Manager::GetCurrent() const { return currentStage_; }

void Manager::RegisterStage(const std::string& name, Stage* stage) {
  if (stages_.contains(name)) {
    throw std::runtime_error("Stage with name " + name + " already registered");
  }
  stage->resources_ = &resources_;
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
    const auto stageChangedEvent =
        event::StageEvent::MakeActiveStageChanged(currentStage_, prevStage);
    if (prevStage != nullptr) {
      prevStage->Handle(stageChangedEvent);
    }
    currentStage_->Handle(stageChangedEvent);
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

void Manager::Handle(const video::event::WindowEvent& event) {
  if (currentStage_ == nullptr) {
    return;
  }
  currentStage_->Handle(event);
}
void Manager::Handle(const soil::event::EngineEvent& event) {
  if (currentStage_ == nullptr) {
    return;
  }
  currentStage_->Handle(event);
}
Resources& Manager::GetResources() const { return resources_; }

Stage* Manager::GetStage(const std::string& name) const {
  const auto itr = stages_.find(name);
  if (itr == stages_.end()) {
    return nullptr;
  }
  return itr->second;
}
void Manager::HookTo(
    soil::event::Observable<soil::event::EngineEvent>& engineEventObservable,
    soil::event::Observable<input::Event>& inputEventObservable,
    soil::event::Observable<video::event::WindowEvent>& windowEventObservable) {
  engineEventObservable.AddListener(this);
  inputEventObservable.AddListener(this);
  windowEventObservable.AddListener(this);
}

}  // namespace soil::stage
