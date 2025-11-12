#include "stage/scene/component/render/render_state_container_component.h"

#include "stage/scene/component/render/renderable_component.h"
#include "stage/scene/node.h"

namespace soil::stage::scene::component::render {
RenderStateContainerComponent::RenderStateContainerComponent(
    video::render::StateContainer *renderStateContainer)
    : RenderComponent(),
      hook_({event::Hook::EventType::Node}),
      renderStateContainer_(renderStateContainer) {
  hook_.SetNodeEventCallback(
      [this](const soil::stage::event::Node &event) { OnEvent(event); });
}

RenderStateContainerComponent::~RenderStateContainerComponent() {
  delete renderStateContainer_;
}

void RenderStateContainerComponent::Render(video::render::State &state) {
  for (auto &id : renderStateContainer_->States()) {
    renderStateContainer_->GroupByState(id)->Render(state);
  }
}

void RenderStateContainerComponent::OnEvent(
    const soil::stage::event::Node &event) {
  if (event.ChangeType == stage::event::Node::ChangeType::Component &&
      event.ComponentEvent.Origin->GetType() == Component::Type::Renderable) {
    Handle(event.ComponentEvent);
    return;
  }

  if (event.ChangeType == stage::event::Node::ChangeType::ChildRemoved) {
    RemoveAllDependentRenderComponents(event.ChangedNode);
  }
  if (event.ChangeType == stage::event::Node::ChangeType::Deleted) {
    RemoveAllDependentRenderComponents(event.Origin);
  }
}

void RenderStateContainerComponent::RemoveAllDependentRenderComponents(
    Node *node) {
  const auto removeVisualComp = [this](auto *component) {
    auto *vComp = dynamic_cast<RenderableComponent *>(component);
    auto *renderable = vComp != nullptr ? vComp->GetRenderable() : nullptr;
    if (renderable == nullptr) {
      return;
    }
    renderStateContainer_->Remove(renderable);
  };
  node->ForEachComponent(removeVisualComp, Component::Type::Renderable);
  Node::ForEachChild(node, [removeVisualComp](Node *child) {
    child->ForEachComponent(removeVisualComp, Component::Type::Renderable);
  });
}

void RenderStateContainerComponent::Handle(
    const stage::event::Component &event) {
  auto *vComp = dynamic_cast<RenderableComponent *>(event.Origin);
  auto *renderable = vComp != nullptr ? vComp->GetRenderable() : nullptr;
  if (renderable == nullptr) {
    return;
  }
  switch (event.Trigger) {
    case stage::event::Component::TriggerType::Added:
      if (!vComp->IsVisible() || vComp->IsCulled()) {
        return;
      }
      renderStateContainer_->Insert(renderable);
      // OnAdded(vComp);
      break;
    case stage::event::Component::TriggerType::Removed:
      renderStateContainer_->Remove(renderable);
      // OnRemoved(vComp);
      break;
    case stage::event::Component::TriggerType::Changed:
      renderStateContainer_->Remove(renderable);
      if (vComp->IsVisible() && !vComp->IsCulled()) {
        renderStateContainer_->Insert(renderable);
      }
      /*if (event.Changed == event::Component::ChangeType::Data) {
        // OnChanged(vComp);
      }*/
      break;
    default:;
  }
}
void RenderStateContainerComponent::OnStageChanged(Stage *stage,
                                                   Stage *prevStage) {
  Component::OnStageChanged(stage, prevStage);
  hook_.SetStage(stage);
}

void RenderStateContainerComponent::SetParent(Node *parent) {
  if (parent_ == parent) {
    return;
  }
  if (parent != nullptr) {
    hook_.SetTriggerRoot(parent->Root());
  }
  RenderComponent::SetParent(parent);
}

}  // namespace soil::stage::scene::component::render
