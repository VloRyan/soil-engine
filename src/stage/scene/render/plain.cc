#include "stage/scene/render/plain.h"

#include <stdexcept>

#include "stage/scene/component/visual_component.h"
#include "video/render/container.h"

namespace soil::stage::scene::render {
Plain::Plain(video::render::Container* renderContainer)
    : hook::TriggerHook({TriggerHook::TriggerType::Render}),
      renderContainer_(renderContainer) {}

void Plain::OnEvent(const event::Node& event) {
  if (event.ChangeType != event::Node::ChangeType::Component) {
    return;
  }
  Handle(event.ComponentEvent);
}

void Plain::Handle(const event::Component& event) {
  if (!component::VisualComponent::IsRenderType(event.Origin,
                                                render::Type::Plain)) {
    return;
  }
  auto* vComp = dynamic_cast<component::VisualComponent*>(event.Origin);
  if (vComp->GetRenderable() == nullptr) {
    return;
  }
  switch (event.Trigger) {
    case event::Component::TriggerType::Added:
      OnAdded(vComp);
      break;
    case event::Component::TriggerType::Removed:
      OnRemoved(vComp);
      break;
    case event::Component::TriggerType::Changed:
      if (event.Changed == event::Component::ChangeType::Data) {
        OnChanged(vComp);
      }
      break;
    default:;
  }
}

void Plain::OnRemoved(component::VisualComponent* component) {
  if (component->GetRenderable()->GetContainerStateIndex() != -1) {
    if (component->IsOpaque()) {
      auto removed = renderContainer_->Remove(component->GetRenderable(),
                                              video::render::Container::OPAQUE);
      if (!removed) {
        renderContainer_->Remove(component->GetRenderable(),
                                 video::render::Container::NON_OPAQUE);
      }
    } else {
      auto removed = renderContainer_->Remove(
          component->GetRenderable(), video::render::Container::NON_OPAQUE);
      if (!removed) {
        renderContainer_->Remove(component->GetRenderable(),
                                 video::render::Container::OPAQUE);
      }
    }
    for (auto itr = changed_.begin(); itr != changed_.end(); ++itr) {
      if (component == *itr) {
        changed_.erase(itr);
        break;
      }
    }
  } else {
    for (auto itr = added_.begin(); itr != added_.end(); ++itr) {
      if (component == *itr) {
        added_.erase(itr);
        break;
      }
    }
  }
  /*if (!removed) {
    throw std::runtime_error("Failed to remove component");
  }*/
}

void Plain::OnAdded(component::VisualComponent* component) {
  if (!component->IsVisible() || component->IsCulled()) {
    return;
  }
  added_.push_back(component);
}

void Plain::OnChanged(component::VisualComponent* component) {
  if (!component->IsVisible() || component->IsCulled()) {
    OnRemoved(component);
    return;
  }
  if (component->GetRenderable()->GetContainerStateIndex() != -1) {
    for (auto itr = changed_.begin(); itr != changed_.end(); ++itr) {
      if (component == *itr) {
        return;
      }
    }
    changed_.push_back(component);
  } else {
    for (auto itr = added_.begin(); itr != added_.end(); ++itr) {
      if (component == *itr) {
        return;
      }
    }
    added_.push_back(component);
  }
}

void Plain::OnTrigger(hook::TriggerHook::TriggerType trigger) {
  for (auto* comp : added_) {
    if (comp->IsOpaque()) {
      renderContainer_->Add(comp->GetRenderable(),
                            video::render::Container::OPAQUE);
    } else {
      renderContainer_->Add(comp->GetRenderable(),
                            video::render::Container::NON_OPAQUE);
    }
  }
  added_.clear();

  for (auto* comp : changed_) {
    auto* renderable = comp->GetRenderable();
    const auto index = renderable->GetContainerStateIndex();
    UpdateOpacity(comp->IsOpaque(), index, renderable);
  }
  changed_.clear();
}

void Plain::UpdateOpacity(bool isOpaque, int index,
                          video::render::RenderableObject* object) {
  std::vector<video::render::RenderableObject*> renderables;
  if (isOpaque) {
    renderContainer_->GetPerDef(renderables, video::render::Container::OPAQUE);
    if (index < renderables.size() && renderables[index] == object) {
      return;  // all good
    }
    if (!renderContainer_->Remove(object,
                                  video::render::Container::NON_OPAQUE)) {
      throw std::runtime_error("failed to remove renderable");
    }
    renderContainer_->Add(object, video::render::Container::OPAQUE);
  } else {
    renderContainer_->GetPerDef(renderables,
                                video::render::Container::NON_OPAQUE);
    if (index < renderables.size() && renderables[index] == object) {
      return;  // all good
    }
    if (!renderContainer_->Remove(object, video::render::Container::OPAQUE)) {
      throw std::runtime_error("failed to remove renderable");
    }
    renderContainer_->Add(object, video::render::Container::NON_OPAQUE);
  }
}
}  // namespace soil::stage::scene::render
