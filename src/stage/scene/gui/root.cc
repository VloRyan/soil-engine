#include "stage/scene/gui/root.h"

#include "stage/stage.h"
#include "video/glfw_window.h"

namespace soil::stage::scene::gui {
Root::Root(const glm::ivec2 windowSize) : overlayContainer_(nullptr) {
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

void Root::addToOverlayContainer(Rectangle* rect) {
  if (overlayContainer_ == nullptr) {
    overlayContainer_ = AddChild(new Rectangle());
    overlayContainer_->SetRelativeSize(glm::vec2(1.F));
    overlayContainer_->SetLocalPosition(
        glm::vec3(0.F, 0.F, TOP_Z_LAYER - 10.F));
  }
  overlayContainer_->AddChild(rect);
}
void Root::RemoveOverlay(Rectangle* rect) {
  if (overlayContainer_ == nullptr) {
    return;
  }
  overlayContainer_->RemoveChild(rect);
}

void Root::FindChildrenAt(std::vector<Rectangle*>& result, glm::ivec2 pos,
                          bool onlyVisible) {
  for (auto* child : childRects_) {
    if (onlyVisible && !child->IsVisible() || !child->Contains(pos)) {
      continue;
    }
    if (child == overlayContainer_) {
      overlayContainer_->FindChildrenAt(result, pos, onlyVisible);
      continue;
    }
    result.push_back(child);
  }
}
}  // namespace soil::stage::scene::gui
