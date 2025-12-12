#include "stage/scene/volume/frustum_culling.h"

#include "util/strings.h"
#include "world/volume/quad_tree.h"

namespace soil::stage::scene::volume {
FrustumCulling::FrustumCulling(viewer::Node* viewer,
                               const component::world::WorldComponent* world)
    : hook::TriggerHook(), world_(world), viewer_(viewer) {}

void FrustumCulling::OnEvent(const event::Node& event) {
  if (event.ChangeType != event::Node::ChangeType::Component ||
      event.ComponentEvent.Trigger != event::Component::TriggerType::Added) {
    return;
  }
  OnComponentAdded(event.ComponentEvent.Origin);
}

void FrustumCulling::OnComponentAdded(component::Component* component) {
  if (component == nullptr ||
      component->GetType() != component::Component::Type::Drawable) {
    return;
  }
  auto* renderable = dynamic_cast<component::DrawableComponent*>(component);
  if (renderable == nullptr) {
    return;
  }
  addedRenderables_.push_back(renderable);
}

void FrustumCulling::OnTrigger(const TriggerPoint& point) {
  if (nodesVisibility_.size() < world_->Container()->GetNodeCount()) {
    // TODO make it more efficient
    nodesVisibility_.resize(world_->Container()->GetNodeCount(), false);
  }
  if (viewer_ == nullptr) {
    return;
  }
  updateVisibilityOnTreeNode(0, viewer_->GetFrustum());
  for (auto* vComp : addedRenderables_) {
    const auto* parent = vComp->GetParent();
    if (!parent->HasComponent(component::Component::Type::WorldEntity)) {
      continue;
    }
    const auto* objectComponent =
        dynamic_cast<const component::world::CollisionObjectComponent*>(
            parent->GetFirstComponent(component::Component::Type::WorldEntity));
    std::vector<int> indices;
    world_->Container()->QueryNodeIndicesFor(objectComponent->Object(),
                                             indices);
    auto visible = false;
    for (const auto index : indices) {
      if (nodesVisibility_[index]) {
        visible = true;
        break;
      }
    }
    vComp->SetCulled(!visible);
  }
  addedRenderables_.clear();
}

void FrustumCulling::updateVisibilityOnTreeNode(
    const int index, const soil::world::volume::Frustum* frustum) {
  const auto* treeNode = world_->Container()->GetNode(index);
  const auto isVisible =
      frustum->IntersectBox({treeNode->Min.x, 0, treeNode->Min.y},
                            {treeNode->Max.x, 0, treeNode->Max.y});
  if (!isVisible && nodesVisibility_[index] == isVisible) {
    return;
  }
  if (nodesVisibility_[index] != isVisible) {
    std::vector<const soil::world::entity::CollisionObject*> objects;
    world_->Container()->GetNodeObjects(index, objects);
    for (const auto* object : objects) {
      auto* objectComponent = world_->ResolveCollisionObjectComponent(object);
      const auto* node = objectComponent->GetParent();
      node->ForEachComponent(
          [isVisible](component::Component* comp) {
            auto* vComp = dynamic_cast<component::DrawableComponent*>(comp);
            vComp->SetCulled(!isVisible);
          },
          component::Component::Type::Drawable);
    }
    nodesVisibility_[index] = isVisible;
    if (!nodesVisibility_[index]) {
      const int childrenStartIndex = treeNode->ChildrenStartIndex;
      if (childrenStartIndex == soil::world::volume::Container::NO_CHILDREN) {
        return;
      }
      for (int j = 0; j < world_->Container()->GetChildrenPerNode(); ++j) {
        if (nodesVisibility_[childrenStartIndex + j] != isVisible) {
          setVisibility(childrenStartIndex + j, false);
        }
      }
      return;
    }
  }
  if (nodesVisibility_[index]) {
    const int childrenStartIndex = treeNode->ChildrenStartIndex;
    if (childrenStartIndex == soil::world::volume::Container::NO_CHILDREN) {
      return;
    }
    for (int j = 0; j < world_->Container()->GetChildrenPerNode(); ++j) {
      updateVisibilityOnTreeNode(childrenStartIndex + j, frustum);
    }
  }
}

void FrustumCulling::setVisibility(const int index, const bool visible) {
  if (nodesVisibility_[index] == visible) {
    return;
  }
  const auto* treeNode = world_->Container()->GetNode(index);
  const int childrenStartIndex = treeNode->ChildrenStartIndex;
  if (childrenStartIndex == soil::world::volume::Container::NO_CHILDREN) {
    return;
  }
  for (int j = 0; j < world_->Container()->GetChildrenPerNode(); ++j) {
    setVisibility(childrenStartIndex + j, visible);
  }
}

}  // namespace soil::stage::scene::volume
