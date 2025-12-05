#include "stage/scene/gui/root.h"

#include "stage/stage.h"
#include "video/glfw_window.h"

namespace soil::stage::scene::gui {
Root::Root(const glm::ivec2 windowSize) {
  size_ = windowSize;
  scissorRect_.Size = size_;
  childScissorRect_.Size = size_;
  const auto parentHalfSize = size_ / glm::ivec2(2);
  Node::SetPosition(glm::vec3(parentHalfSize, -TOP_Z_LAYER));
}

void Root::OnStageChanged(soil::stage::Stage* stage,
                          soil::stage::Stage* prevStage) {
  Node::OnStageChanged(stage, prevStage);
  if (prevStage != nullptr) {
    prevStage->RemoveEventHook(static_cast<EventHook<input::Event>*>(this));
    prevStage->RemoveEventHook(
        static_cast<EventHook<soil::video::event::WindowEvent>*>(this));
  }
  if (stage != nullptr) {
    stage->AddEventHook(static_cast<EventHook<input::Event>*>(this));
    stage->AddEventHook(
        static_cast<EventHook<soil::video::event::WindowEvent>*>(this));
  }
}

void Root::OnEvent(const input::Event& event) {
  if (event.Origin == input::Event::OriginType::MouseButton ||
      event.Origin == input::Event::OriginType::MousePosition ||
      event.Origin == input::Event::OriginType::MouseWheel) {
    const auto pos =
        glm::ivec2(event.CursorPos.x,
                   size_.y - event.CursorPos.y);  // origin to bottom-left
    if (!Contains(pos)) {
      if (isMouseOver_) {
        OnMouseOut();
      }
      return;
    }
    switch (event.Origin) {
      case input::Event::OriginType::MouseButton:
        OnMouseButton(pos, event.MouseButton, event.State);
        break;
      case input::Event::OriginType::MousePosition:
        OnMouseOver(pos);
        break;
      case input::Event::OriginType::MouseWheel:
        OnMouseWheel(pos, event.WheelOffset);
      default:;
    }
  }
}

void Root::OnEvent(const soil::video::event::WindowEvent& event) {
  if (event.Cause != soil::video::event::WindowEvent::SizeChanged) {
    return;
  }
  size_ = event.Window->GetSize();
  scissorRect_.Size = size_;
  childScissorRect_.Size = size_;
  const auto parentHalfSize = size_ / glm::ivec2(2);
  SetPosition(glm::vec3(parentHalfSize, -TOP_Z_LAYER));
  SetDirty(DirtyImpact::Dependents);
}

bool Root::IsOnElement(glm::ivec2 pos) {
  for (auto* child : children_) {
    if (child->IsVisible() && child->Contains(pos)) {
      return true;
    }
  }
  return false;
}
}  // namespace soil::stage::scene::gui
