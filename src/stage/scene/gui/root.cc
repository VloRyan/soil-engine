#include "stage/scene/gui/root.h"

#include "stage/stage.h"
#include "window.h"

namespace soil::stage::scene::gui {
Root::Root(const glm::ivec2 windowSize) {
  size_ = windowSize;
  scissorRect_.Size = size_;
  childScissorRect_.Size = size_;
  // Node::SetReceiverType(ReceiverType::Input, true);
  // Node::SetReceiverType(ReceiverType::Window, true);
  const auto parentHalfSize = size_ / glm::ivec2(2);
  Node::SetPosition(glm::vec3(parentHalfSize, -TOP_Z_LAYER));
}

void Root::OnStageChanged(soil::stage::Stage* stage,
                          soil::stage::Stage* prevStage) {
  Node::OnStageChanged(stage, prevStage);
  if (prevStage != nullptr) {
    prevStage->RemoveEventHook(static_cast<EventHook<input::Event>*>(this));
    prevStage->RemoveEventHook(static_cast<EventHook<WindowEvent>*>(this));
  }
  if (stage != nullptr) {
    stage->AddEventHook(static_cast<EventHook<input::Event>*>(this));
    stage->AddEventHook(static_cast<EventHook<WindowEvent>*>(this));
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
  Node::Handle(event);
}

void Root::OnEvent(const WindowEvent& event) {
  if (event.Cause != WindowEvent::SizeChanged) {
    return;
  }
  size_ = event.Window->GetSize();
  scissorRect_.Size = size_;
  childScissorRect_.Size = size_;
  const auto parentHalfSize = size_ / glm::ivec2(2);
  SetPosition(glm::vec3(parentHalfSize, -TOP_Z_LAYER));
  SetDirty(DirtyImpact::Dependents);
  Node::Handle(event);
}

}  // namespace soil::stage::scene::gui
