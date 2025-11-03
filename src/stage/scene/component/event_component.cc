#include "stage/scene/component/event_component.h"

#include "stage/stage.h"
namespace soil::stage::scene::component {
EventComponent::EventComponent(
    const std::vector<EventType> &events,
    const std::vector<soil::stage::hook::TriggerHook::TriggerType> &triggers)
    : Component(Component::Type::Lighting),
      hook::TriggerHook(triggers),
      activeTrigger_(!triggers.empty()) {
  activeEvents_ = events;
}

void EventComponent::OnEvent(const event::Node &event) {}
void EventComponent::OnEvent(const input::Event &event) {}
void EventComponent::OnEvent(const WindowEvent &event) {}
void EventComponent::OnTrigger(
    soil::stage::hook::TriggerHook::TriggerType trigger) {}

void EventComponent::OnStageChanged(soil::stage::Stage *stage,
                                    soil::stage::Stage *prevStage) {
  Component::OnStageChanged(stage, prevStage);
  if (stage_ != nullptr) {
    for (auto event : activeEvents_) {
      switch (event) {
        case EventType::Node:
          stage_->RemoveEventHook(
              static_cast<hook::EventHook<event::Node> *>(this));
          break;
        case EventType::Window:
          stage_->RemoveEventHook(
              static_cast<hook::EventHook<input::Event> *>(this));
          break;
        case EventType::Input:
          stage_->RemoveEventHook(
              static_cast<hook::EventHook<WindowEvent> *>(this));
          break;
      }
    }
    if (activeTrigger_) {
      stage_->RemoveTriggerHook(this);
    }
  }
  stage_ = stage;
  if (stage_ != nullptr) {
    for (auto event : activeEvents_) {
      switch (event) {
        case EventType::Node:
          stage_->AddEventHook(
              static_cast<hook::EventHook<event::Node> *>(this));
          break;
        case EventType::Window:
          stage_->AddEventHook(
              static_cast<hook::EventHook<input::Event> *>(this));
          break;
        case EventType::Input:
          stage_->AddEventHook(
              static_cast<hook::EventHook<WindowEvent> *>(this));
          break;
      }
    }
    if (activeTrigger_) {
      stage_->AddTriggerHook(this);
    }
  }
}

void EventComponent::ActivateEvents(const std::vector<EventType> &events) {
  for (auto event : activeEvents_) {
    switch (event) {
      case EventType::Node:
        stage_->RemoveEventHook(
            static_cast<hook::EventHook<event::Node> *>(this));
        break;
      case EventType::Window:
        stage_->RemoveEventHook(
            static_cast<hook::EventHook<input::Event> *>(this));
        break;
      case EventType::Input:
        stage_->RemoveEventHook(
            static_cast<hook::EventHook<WindowEvent> *>(this));
        break;
    }
  }
  activeEvents_ = events;
  for (auto event : activeEvents_) {
    switch (event) {
      case EventType::Node:
        stage_->AddEventHook(static_cast<hook::EventHook<event::Node> *>(this));
        break;
      case EventType::Window:
        stage_->AddEventHook(
            static_cast<hook::EventHook<input::Event> *>(this));
        break;
      case EventType::Input:
        stage_->AddEventHook(static_cast<hook::EventHook<WindowEvent> *>(this));
        break;
    }
  }
}
void EventComponent::ActivateTrigger(
    const std::vector<soil::stage::hook::TriggerHook::TriggerType> &triggers) {
  if (activeTrigger_) {
    stage_->RemoveTriggerHook(this);
  }
  SetTriggers(triggers);
  if (!triggers.empty()) {
    stage_->AddTriggerHook(this);
  }
}

}  // namespace soil::stage::scene::component
