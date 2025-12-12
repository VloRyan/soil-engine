#include "stage/scene/component/event/hook.h"

#include "stage/scene/node.h"
#include "stage/stage.h"
namespace soil::stage::scene::component::event {
Hook::Hook(
    const std::vector<EventType> &events,
    const std::vector<soil::stage::hook::TriggerHook::TriggerPoint> &points)
    : hook::TriggerHook(),
      activeTriggerPoints_(points),
      activeEvents_(events),
      stage_(nullptr),
      nodeEventCallback_(nullptr),
      inputEventCallback_(nullptr),
      windowEventCallback_(nullptr),
      gameEventCallback_(nullptr),
      triggerCallback_(nullptr),
      triggerRoot_(nullptr) {}

Hook::~Hook() { SetStage(nullptr); }

void Hook::OnEvent(const stage::event::Node &event) {
  if (nodeEventCallback_ != nullptr) {
    nodeEventCallback_(event);
  }
}

void Hook::OnEvent(const input::Event &event) {
  if (inputEventCallback_ != nullptr) {
    inputEventCallback_(event);
  }
}

void Hook::OnEvent(const soil::video::event::WindowEvent &event) {
  if (windowEventCallback_ != nullptr) {
    windowEventCallback_(event);
  }
}

void Hook::OnEvent(const stage::event::GameEvent &event) {
  if (gameEventCallback_ != nullptr) {
    gameEventCallback_(event);
  }
}
void Hook::OnEvent(const soil::event::EngineEvent &event) {
  if (engineEventCallback_ != nullptr) {
    engineEventCallback_(event);
  }
}

void Hook::OnTrigger(const TriggerPoint &point) {
  if (triggerCallback_ != nullptr) {
    triggerCallback_(point);
  }
}

void Hook::SetStage(soil::stage::Stage *stage) {
  if (stage_ == stage) {
    return;
  }
  if (stage_ != nullptr) {
    for (auto event : activeEvents_) {
      switch (event) {
        case EventType::Node:
          stage_->RemoveEventHook(
              triggerRoot_,
              static_cast<hook::EventHook<stage::event::Node> *>(this));
          break;
        case EventType::Input:
          stage_->RemoveEventHook(
              static_cast<hook::EventHook<input::Event> *>(this));
          break;
        case EventType::Window:
          stage_->RemoveEventHook(
              static_cast<hook::EventHook<soil::video::event::WindowEvent> *>(
                  this));
          break;
        case EventType::Game:
          stage_->RemoveEventHook(
              static_cast<hook::EventHook<stage::event::GameEvent> *>(this));
        case EventType::Engine:
          stage_->RemoveEventHook(
              static_cast<hook::EventHook<soil::event::EngineEvent> *>(this));
          break;
      }
    }
    for (auto point : activeTriggerPoints_) {
      stage_->RemoveTriggerHook(this, point);
    }
  }
  stage_ = stage;
  if (stage_ != nullptr) {
    for (auto event : activeEvents_) {
      switch (event) {
        case EventType::Node:
          stage_->AddEventHook(
              triggerRoot_,
              static_cast<hook::EventHook<stage::event::Node> *>(this));
          break;
        case EventType::Input:
          stage_->AddEventHook(
              static_cast<hook::EventHook<input::Event> *>(this));
          break;
        case EventType::Window:
          stage_->AddEventHook(
              static_cast<hook::EventHook<soil::video::event::WindowEvent> *>(
                  this));
          break;
        case EventType::Game:
          stage_->AddEventHook(
              static_cast<hook::EventHook<stage::event::GameEvent> *>(this));
          break;
        case EventType::Engine:
          stage_->AddEventHook(
              static_cast<hook::EventHook<soil::event::EngineEvent> *>(this));
          break;
      }
    }
    for (auto point : activeTriggerPoints_) {
      stage_->AddTriggerHook(this, point);
    }
  } else {
    // erase all points with root
    for (auto itr = activeTriggerPoints_.begin();
         itr != activeTriggerPoints_.end();) {
      if (itr->Root != nullptr) {
        itr = activeTriggerPoints_.erase(itr);
        continue;
      }
      ++itr;
    }
  }
}

void Hook::ActivateEvents(const std::vector<EventType> &events) {
  if (stage_ != nullptr) {
    for (auto event : activeEvents_) {
      switch (event) {
        case EventType::Node:
          stage_->RemoveEventHook(
              triggerRoot_,
              static_cast<hook::EventHook<stage::event::Node> *>(this));
          break;
        case EventType::Input:
          stage_->RemoveEventHook(
              static_cast<hook::EventHook<input::Event> *>(this));
          break;
        case EventType::Window:
          stage_->RemoveEventHook(
              static_cast<hook::EventHook<soil::video::event::WindowEvent> *>(
                  this));
          break;
        case EventType::Game:
          stage_->RemoveEventHook(
              static_cast<hook::EventHook<stage::event::GameEvent> *>(this));
          break;
        case EventType::Engine:
          stage_->RemoveEventHook(
              static_cast<hook::EventHook<soil::event::EngineEvent> *>(this));
          break;
      }
    }
  }
  activeEvents_ = events;
  if (stage_ != nullptr) {
    for (auto event : activeEvents_) {
      switch (event) {
        case EventType::Node:
          stage_->AddEventHook(
              triggerRoot_,
              static_cast<hook::EventHook<stage::event::Node> *>(this));
          break;
        case EventType::Input:
          stage_->AddEventHook(
              static_cast<hook::EventHook<input::Event> *>(this));
          break;
        case EventType::Window:
          stage_->AddEventHook(
              static_cast<hook::EventHook<soil::video::event::WindowEvent> *>(
                  this));
          break;
        case EventType::Game:
          stage_->AddEventHook(
              static_cast<hook::EventHook<stage::event::GameEvent> *>(this));
          break;
        case EventType::Engine:
          stage_->AddEventHook(
              static_cast<hook::EventHook<soil::event::EngineEvent> *>(this));
          break;
      }
    }
  }
}

void Hook::ActivateTrigger(const TriggerPoint &point) {
  for (auto activePoint : activeTriggerPoints_) {
    if (activePoint == point) {
      return;
    }
  }
  if (stage_ != nullptr) {
    stage_->AddTriggerHook(this, point);
  }
  activeTriggerPoints_.push_back(point);
}

void Hook::RemoveTrigger(const hook::TriggerHook::TriggerPoint &point) {
  for (auto itr = activeTriggerPoints_.begin();
       itr != activeTriggerPoints_.end(); ++itr) {
    if (*itr == point) {
      if (stage_ != nullptr) {
        stage_->RemoveTriggerHook(this, point);
      }
      return;
    }
  }
}
void Hook::SetNodeEventCallback(
    const std::function<void(const stage::event::Node &)> &nodeEventCallback) {
  nodeEventCallback_ = nodeEventCallback;
}

void Hook::SetInputEventCallback(
    const std::function<void(const input::Event &)> &inputEventCallback) {
  inputEventCallback_ = inputEventCallback;
}

void Hook::SetWindowEventCallback(
    const std::function<void(const soil::video::event::WindowEvent &)>
        &windowEventCallback) {
  windowEventCallback_ = windowEventCallback;
}

void Hook::SetTriggerCallback(
    const std::function<void(const TriggerPoint &point)> &triggerCallback) {
  triggerCallback_ = triggerCallback;
}

void Hook::SetGameEventCallback(
    const std::function<void(const stage::event::GameEvent &)>
        &gameEventCallback) {
  gameEventCallback_ = gameEventCallback;
}

void Hook::SetEngineEventCallback(
    const std::function<void(const soil::event::EngineEvent &)>
        &engineEventCallback) {
  engineEventCallback_ = engineEventCallback;
}

void Hook::SetTriggerRoot(Node *triggerRoot) {
  if (triggerRoot_ == triggerRoot) {
    return;
  }
  if (stage_ == nullptr) {
    triggerRoot_ = triggerRoot;
    return;
  }
  for (auto event : activeEvents_) {
    switch (event) {
      case EventType::Node:
        stage_->RemoveEventHook(
            triggerRoot_,
            static_cast<hook::EventHook<stage::event::Node> *>(this));
        break;
      default:;
    }
  }
  for (auto point : activeTriggerPoints_) {
    stage_->RemoveTriggerHook(this, point);
  }
  triggerRoot_ = triggerRoot;
  for (auto event : activeEvents_) {
    switch (event) {
      case EventType::Node:
        stage_->AddEventHook(
            triggerRoot_,
            static_cast<hook::EventHook<stage::event::Node> *>(this));
        break;
      default:;
    }
  }
  for (auto point : activeTriggerPoints_) {
    point.Root = triggerRoot_;
    stage_->AddTriggerHook(this, point);
  }
}
stage::scene::Node *Hook::TriggerRoot() { return triggerRoot_; }

}  // namespace soil::stage::scene::component::event
