
#include "stage/stage.h"

#include "stage/scene/scene.h"

namespace soil::stage {
Stage::Stage() : manager_(nullptr), loaded_(false), resources_(nullptr) {}

Stage::~Stage() {
  if (IsLoaded()) {
    Stage::Unload();
  }
  for (auto* scene : scenes_) {
    scene->stage_ = nullptr;
    delete scene;
  }
}

void Stage::Update() {
  triggerHooks(hook::TriggerHook::TriggerType::BeforeUpdateScene);
  for (auto* scene : scenes_) {
    scene->Update();
  }
  triggerHooks(hook::TriggerHook::TriggerType::AfterUpdateScene);
}

void Stage::Render(video::render::State& state) {
  triggerHooks(hook::TriggerHook::TriggerType::Render);
  for (auto* scene : scenes_) {
    scene->Render(state);
  }
}

void Stage::_addScene(scene::Scene* scene) {
  scene->SetStage(this);
  scenes_.emplace_back(scene);
}

std::vector<scene::Scene*> Stage::GetScenes() const { return scenes_; }

IManager* Stage::Manager() const { return manager_; }

void Stage::Handle(const input::Event& event) {
  /*for (auto* scene : scenes_) {
    scene->Handle(event);
  }*/
  for (auto* hook : inputEventHooks_) {
    hook->OnEvent(event);
  }
}

void Stage::Handle(const WindowEvent& event) {
  /*for (auto* scene : scenes_) {
    scene->Handle(event);
  }*/
  for (auto* hook : windowEventHooks_) {
    hook->OnEvent(event);
  }
}

void Stage::Handle(const event::Node& event) {
  for (auto* hook : nodeEventHooks_) {
    hook->OnEvent(event);
  }
}

void Stage::Handle(const event::StageEvent& event) {
  if (event.Trigger == event::StageEvent::TriggerType::ActiveStageChanged &&
      event.Stage == this && IsLoaded()) {
    const auto winEvent =
        WindowEvent(GetResources().GetWindow(), WindowEvent::SizeChanged);
    Handle(winEvent);
  }
}

void Stage::SetCurrent() { manager_->SetCurrent(this); }

void Stage::RemoveScene(const scene::Scene* scene) {
  for (auto itr = scenes_.begin(); itr != scenes_.end(); ++itr) {
    if (*itr == scene) {
      scenes_.erase(itr);
      break;
    }
  }
}

Resources& Stage::GetResources() const {
  if (resources_ == nullptr) {
    throw StageNotRegisteredException();
  }
  return *resources_;
}

void Stage::Load() { loaded_ = true; }

void Stage::Unload() { loaded_ = false; }

bool Stage::IsLoaded() const { return loaded_; }

void Stage::AddTriggerHook(soil::stage::hook::TriggerHook* trigger) {
  for (auto typeIdx = 0;
       typeIdx !=
       static_cast<int>(soil::stage::hook::TriggerHook::TriggerType::COUNT);
       typeIdx++) {
    auto type =
        static_cast<soil::stage::hook::TriggerHook::TriggerType>(typeIdx);
    if (trigger->IsTrigger(type)) {
      auto itr = triggerHooks_.find(type);
      if (itr == triggerHooks_.end()) {
        triggerHooks_.insert({type, {trigger}});
      } else {
        itr->second.push_back(trigger);
      }
    }
  }
}
void Stage::RemoveTriggerHook(soil::stage::hook::TriggerHook* trigger) {
  for (auto typeIdx = 0;
       typeIdx !=
       static_cast<int>(soil::stage::hook::TriggerHook::TriggerType::COUNT);
       typeIdx++) {
    auto type =
        static_cast<soil::stage::hook::TriggerHook::TriggerType>(typeIdx);
    if (trigger->IsTrigger(type)) {
      auto itr = triggerHooks_.find(type);
      if (itr != triggerHooks_.end()) {
        for (auto triggerItr = itr->second.begin();
             triggerItr != itr->second.end(); ++itr) {
          if (*triggerItr == trigger) {
            itr->second.erase(triggerItr);
          }
        }
      }
    }
  }
}
void Stage::triggerHooks(soil::stage::hook::TriggerHook::TriggerType type) {
  auto itr = triggerHooks_.find(type);
  if (itr != triggerHooks_.end()) {
    for (auto* hook : itr->second) {
      hook->OnTrigger(type);
    }
  }
}
/*
void Stage::AddEventHook(
    soil::stage::scene::hook::EventHook<soil::event::Event>* hook) {
  auto itr = eventHooks_.find(typeid(hook));
  if (itr != eventHooks_.end()) {
    itr->second.push_back(hook);
  } else {
    eventHooks_.insert({typeid(hook), {hook}});
  }
}*/
}  // namespace soil::stage
