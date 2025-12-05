
#include "stage/stage.h"

#include "stage/scene/scene.h"

namespace soil::stage {
Stage::Stage()
    : manager_(nullptr),
      loaded_(false),
      resources_(nullptr),
      triggering_(false) {}

Stage::~Stage() {
  if (IsLoaded()) {
    Stage::Unload();
  }
  triggerHooks_.clear();
  for (auto* scene : scenes_) {
    scene->stage_ = nullptr;
    delete scene;
  }
}

void Stage::Update() {
  triggerHooks(
      {.TriggerType = hook::TriggerHook::TriggerType::BeforeUpdateScene});
  for (auto* scene : scenes_) {
    triggerHooks({
        .Root = scene,
        .TriggerType = hook::TriggerHook::TriggerType::BeforeUpdateScene,
    });
    scene->Update();
    triggerHooks({
        .Root = scene,
        .TriggerType = hook::TriggerHook::TriggerType::AfterUpdateScene,
    });
  }
  triggerHooks(
      {.TriggerType = hook::TriggerHook::TriggerType::AfterUpdateScene});
}

void Stage::Render(video::render::State& state) {
  triggerHooks({.TriggerType = hook::TriggerHook::TriggerType::BeforeRender});
  for (auto* scene : scenes_) {
    triggerHooks({
        .Root = scene,
        .TriggerType = hook::TriggerHook::TriggerType::BeforeRender,
    });
    scene->Render(state);
  }
}

void Stage::_addScene(scene::Scene* scene) {
  scene->SetStage(this);
  scenes_.emplace_back(scene);
}

std::vector<scene::Scene*> Stage::GetScenes() const { return scenes_; }

IManager* Stage::Manager() const { return manager_; }
void Stage::AddEventHook(scene::Node* root,
                         soil::stage::hook::EventHook<event::Node>* hook) {
  auto itr = nodeEventHooks_.find(root);
  if (itr == nodeEventHooks_.end()) {
    nodeEventHooks_.insert({root, {hook}});
  } else {
    itr->second.push_back(hook);
  }
}
void Stage::AddEventHook(soil::stage::hook::EventHook<input::Event>* hook) {
  for (auto itr = inputEventHooks_.begin(); itr != inputEventHooks_.end();
       ++itr) {
    if (hook == *itr) {
      return;
    }
  }
  inputEventHooks_.push_back(hook);
}

void Stage::AddEventHook(
    soil::stage::hook::EventHook<soil::video::event::WindowEvent>* hook) {
  for (auto itr = windowEventHooks_.begin(); itr != windowEventHooks_.end();
       ++itr) {
    if (hook == *itr) {
      return;
    }
  }
  windowEventHooks_.push_back(hook);
}

void Stage::AddEventHook(soil::stage::hook::EventHook<event::GameEvent>* hook) {
  for (auto itr = gameEventHooks_.begin(); itr != gameEventHooks_.end();
       ++itr) {
    if (hook == *itr) {
      return;
    }
  }
  gameEventHooks_.push_back(hook);
}

void Stage::AddEventHook(
    soil::stage::hook::EventHook<soil::event::EngineEvent>* hook) {
  for (auto itr = engineEventHooks_.begin(); itr != engineEventHooks_.end();
       ++itr) {
    if (hook == *itr) {
      return;
    }
  }
  engineEventHooks_.push_back(hook);
}

void Stage::RemoveEventHook(scene::Node* root,
                            soil::stage::hook::EventHook<event::Node>* hook) {
  _removeEventHook(root, hook, nodeEventHooks_);
}

void Stage::RemoveEventHook(soil::stage::hook::EventHook<input::Event>* hook) {
  for (auto itr = inputEventHooks_.begin(); itr != inputEventHooks_.end();
       ++itr) {
    if (hook == *itr) {
      inputEventHooks_.erase(itr);
      return;
    }
  }
}

void Stage::RemoveEventHook(
    soil::stage::hook::EventHook<soil::video::event::WindowEvent>* hook) {
  for (auto itr = windowEventHooks_.begin(); itr != windowEventHooks_.end();
       ++itr) {
    if (hook == *itr) {
      windowEventHooks_.erase(itr);
      return;
    }
  }
}

void Stage::RemoveEventHook(
    soil::stage::hook::EventHook<event::GameEvent>* hook) {
  for (auto itr = gameEventHooks_.begin(); itr != gameEventHooks_.end();
       ++itr) {
    if (hook == *itr) {
      gameEventHooks_.erase(itr);
      return;
    }
  }
}

void Stage::RemoveEventHook(
    soil::stage::hook::EventHook<soil::event::EngineEvent>* hook) {
  for (auto itr = engineEventHooks_.begin(); itr != engineEventHooks_.end();
       ++itr) {
    if (hook == *itr) {
      engineEventHooks_.erase(itr);
      return;
    }
  }
}

void Stage::Handle(const input::Event& event) {
  for (auto* hook : inputEventHooks_) {
    hook->OnEvent(event);
  }
}

void Stage::Handle(const soil::video::event::WindowEvent& event) {
  for (auto* hook : windowEventHooks_) {
    hook->OnEvent(event);
  }
}

void Stage::Handle(const event::Node& event) {
  auto globals = nodeEventHooks_.find(nullptr);
  if (globals != nodeEventHooks_.end()) {
    for (auto* hook : globals->second) {
      hook->OnEvent(event);
    }
  }
  auto roots = nodeEventHooks_.find(event.Origin->Root());
  if (roots != nodeEventHooks_.end()) {
    for (auto* hook : roots->second) {
      hook->OnEvent(event);
    }
  }
}

void Stage::Handle(const event::GameEvent& event) {
  for (auto* hook : gameEventHooks_) {
    hook->OnEvent(event);
  }
}
void Stage::Handle(const soil::event::EngineEvent& event) {
  for (auto* hook : engineEventHooks_) {
    hook->OnEvent(event);
  }
}

void Stage::Handle(const event::StageEvent& event) {
  if (event.Trigger == event::StageEvent::TriggerType::ActiveStageChanged &&
      event.Stage == this && IsLoaded()) {
    const auto winEvent = video::event::WindowEvent(
        GetResources().GetWindow(), video::event::WindowEvent::SizeChanged);
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

void Stage::AddTriggerHook(
    soil::stage::hook::TriggerHook* trigger,
    const soil::stage::hook::TriggerHook::TriggerPoint& at) {
  auto itr = triggerHooks_.find(at);
  if (itr == triggerHooks_.end()) {
    triggerHooks_.insert({at, {trigger}});
  } else {
    itr->second.push_back(trigger);
  }
}

void Stage::RemoveTriggerHook(
    soil::stage::hook::TriggerHook* hook,
    const soil::stage::hook::TriggerHook::TriggerPoint& at) {
  auto itr = triggerHooks_.find(at);
  if (itr == triggerHooks_.end()) {
    return;
  }
  auto& hooks = itr->second;
  for (auto hookItr = hooks.begin(); hookItr != hooks.end(); ++hookItr) {
    if (*hookItr == hook) {
      hooks.erase(hookItr);
      if (triggering_) {
        invalidatedHooks_.push_back(hook);
      }
      if (hooks.empty()) {
        triggerHooks_.erase(itr);
      }
      break;
    }
  }
}

void Stage::triggerHooks(
    const soil::stage::hook::TriggerHook::TriggerPoint& at) {
  auto itr = triggerHooks_.find(at);
  if (itr == triggerHooks_.end()) {
    return;
  }
  triggering_ = true;
  auto triggers = itr->second;  // copy
  for (auto* hook : triggers) {
    bool isInvalid = false;
    for (auto* invalidHook : invalidatedHooks_) {
      if (hook == invalidHook) {
        isInvalid = true;
        break;
      }
    }
    if (isInvalid) {
      continue;
    }
    hook->OnTrigger(at);
  }
  triggering_ = false;
  invalidatedHooks_.clear();
}

}  // namespace soil::stage
