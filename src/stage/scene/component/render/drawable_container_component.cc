#include "stage/scene/component/render/drawable_container_component.h"

#include "stage/scene/component/render/drawable_component.h"
#include "stage/scene/node.h"
#include "video/render/forward/forward_rendering.h"

namespace soil::stage::scene::component::render {
DrawableContainerComponent::DrawableContainerComponent(
    video::render::Algorythm *rendering)
    : DrawableContainerComponent(
          rendering, new video::render::draw::DrawableContainer()) {}

DrawableContainerComponent::DrawableContainerComponent(
    video::render::Algorythm *rendering,
    video::render::draw::DrawableContainer *container)
    : RenderComponent(),
      hook_({event::Hook::EventType::Node}),
      container_(container),
      rendering_(rendering) {
  hook_.SetNodeEventCallback(
      [this](const soil::stage::event::Node &event) { OnEvent(event); });
}

DrawableContainerComponent::~DrawableContainerComponent() { delete container_; }

void DrawableContainerComponent::Render(video::render::State &state) {
  rendering_->Render(state, *container_);
}

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
  auto *renderComp = dynamic_cast<DrawableComponent *>(event.Origin);
  auto *renderable = renderComp != nullptr ? renderComp->Drawable() : nullptr;
  if (renderable == nullptr) {
    return;
  }
  switch (event.Trigger) {
    case stage::event::Component::TriggerType::Added:
      /* if (renderComp->IsDrawablePile()) {
         if (renderable->Container() != nullptr) {
           return;
         }
         container_->Insert(renderable);
       } else {*/
      if (!renderComp->IsVisible() || renderComp->IsCulled() ||
          renderable->Container() != nullptr) {
        return;
      }
      container_->Insert(renderable);
      //}
      break;
    case stage::event::Component::TriggerType::Removed:
      if (renderComp->IsDrawablePile()) {
        return;
      }
      container_->Remove(renderable);
      break;
    case stage::event::Component::TriggerType::Changed:
      if (renderComp->IsDrawablePile()) {
        if (renderable->Container() == nullptr) {
          if (renderComp->IsVisible() && !renderComp->IsCulled()) {
            container_->Insert(renderable);
          }
          return;
        }
      } else {
        if (renderComp->IsVisible() && !renderComp->IsCulled() &&
            renderable->Container() == nullptr) {
          container_->Insert(renderable);
        }
        if ((!renderComp->IsVisible() || renderComp->IsCulled()) &&
            renderable->Container() == container_) {
          container_->Remove(renderable);
        }
      }
      /* if (renderComp->IsDrawablePile() && renderable->Container() != nullptr)
       { return;
       }
       container_->Remove(renderable);
       if (renderComp->IsVisible() && !renderComp->IsCulled()) {
         container_->Insert(renderable);
       }*/
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
  RenderComponent::SetParent(parent);
}

}  // namespace soil::stage::scene::component::render
