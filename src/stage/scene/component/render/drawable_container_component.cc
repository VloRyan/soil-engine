#include "stage/scene/component/render/drawable_container_component.h"

#include "stage/scene/component/render/drawable_component.h"
#include "stage/scene/node.h"
#include "video/render/forward/forward_rendering.h"

namespace soil::stage::scene::component::render {
DrawableContainerComponent::DrawableContainerComponent()
    : DrawableContainerComponent(new video::render::draw::DrawableContainer()) {
}

DrawableContainerComponent::DrawableContainerComponent(
    video::render::draw::DrawableContainer *container)
    : Component(Component::Type::Container),
      hook_({event::Hook::EventType::Node}),
      container_(container) {
  hook_.SetNodeEventCallback(
      [this](const soil::stage::event::Node &event) { OnEvent(event); });
}

DrawableContainerComponent::~DrawableContainerComponent() { delete container_; }

void DrawableContainerComponent::OnEvent(
    const soil::stage::event::Node &event) {
  if (event.ChangeType == stage::event::Node::ChangeType::Component &&
      event.ComponentEvent.Origin->GetType() == Component::Type::Drawable) {
    Handle(event.ComponentEvent);
    return;
  }

  if (event.ChangeType == stage::event::Node::ChangeType::ChildRemoved) {
    RemoveAllDependentDrawableComponents(event.ChangedNode);
  }
  if (event.ChangeType == stage::event::Node::ChangeType::Deleted) {
    RemoveAllDependentDrawableComponents(event.Origin);
  }
}

void DrawableContainerComponent::RemoveAllDependentDrawableComponents(
    Node *node) {
  const auto removeDrawableComp = [this](auto *component) {
    auto *renderComp = dynamic_cast<DrawableComponent *>(component);
    auto *renderable = renderComp != nullptr ? renderComp->Drawable() : nullptr;
    if (renderable == nullptr) {
      return;
    }
    if (renderComp->IsDrawablePile()) {
      return;
    }
    container_->Remove(renderable);
  };
  node->ForEachComponent(removeDrawableComp, Component::Type::Drawable);
  Node::ForEachChild(node, [removeDrawableComp](Node *child) {
    child->ForEachComponent(removeDrawableComp, Component::Type::Drawable);
  });
}

void DrawableContainerComponent::Handle(const stage::event::Component &event) {
  auto *drawableComp = dynamic_cast<DrawableComponent *>(event.Origin);
  auto *drawable = drawableComp != nullptr ? drawableComp->Drawable() : nullptr;
  if (drawable == nullptr) {
    return;
  }
  switch (event.Trigger) {
    case stage::event::Component::TriggerType::Added:
      if (!drawableComp->IsVisible() || drawableComp->IsCulled() ||
          drawable->Container() != nullptr) {
        return;
      }
      container_->Insert(drawable);
      //}
      break;
    case stage::event::Component::TriggerType::Removed:
      if (drawableComp->IsDrawablePile()) {
        return;
      }
      container_->Remove(drawable);
      break;
    case stage::event::Component::TriggerType::Changed:
      // TODO: use change detail
      /*auto changeDetail =
          static_cast<DrawableComponent::ChangeDetails>(event.ChangeDetail);*/
      if (drawableComp->IsDrawablePile()) {
        if (drawable->Container() == nullptr) {
          if (drawableComp->IsVisible() && !drawableComp->IsCulled()) {
            container_->Insert(drawable);
          }
          return;
        }
      } else {
        if (drawableComp->IsVisible() && !drawableComp->IsCulled() &&
            drawable->Container() == nullptr) {
          container_->Insert(drawable);
        }
        if ((!drawableComp->IsVisible() || drawableComp->IsCulled()) &&
            drawable->Container() == container_) {
          container_->Remove(drawable);
        }
      }
      break;
    default:;
  }
}
void DrawableContainerComponent::OnStageChanged(Stage *stage,
                                                Stage *prevStage) {
  Component::OnStageChanged(stage, prevStage);
  hook_.SetStage(stage);
}

void DrawableContainerComponent::SetParent(Node *parent) {
  if (parent_ == parent) {
    return;
  }
  if (parent != nullptr) {
    hook_.SetTriggerRoot(parent->Root());
  }
  Component::SetParent(parent);
}
video::render::draw::DrawableContainer &
DrawableContainerComponent::Container() {
  return *container_;
}

}  // namespace soil::stage::scene::component::render
