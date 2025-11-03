#include "stage/scene/component/update_graph_component.h"

#include "stage/scene/node.h"
#include "stage/scene/scene.h"
#include "stage/stage.h"

namespace soil::stage::scene::component {
UpdateGraphComponent::UpdateGraphComponent()
    : EventComponent({EventComponent::EventType::Node}),
      dirtyNodesPtr_{&dirtyNodesFront_} {
  Component::SetUpdateType(UpdateType::Always);
}

void UpdateGraphComponent::Update() {
  auto* lastDirtyNodes = dirtyNodesPtr_;
  if (dirtyNodesPtr_ == &dirtyNodesFront_) {
    dirtyNodesPtr_ = &dirtyNodesBack_;
  } else {
    dirtyNodesPtr_ = &dirtyNodesFront_;
  }
  for (auto* node : *lastDirtyNodes) {
    if (!node->IsDirty()) {
      continue;
    }
    computeTopDirtyNode(node)->Update();
  }
  lastDirtyNodes->clear();
}

void UpdateGraphComponent::OnEvent(const event::Node& event) {
  switch (event.ChangeType) {
    case event::Node::ChangeType::State:
      OnNodeStateChanged(event.Origin);
      break;
    case event::Node::ChangeType::ChildAdded:
      OnNodeAdded(event.ChangedNode);
      Node::ForEachChild(event.ChangedNode,
                         [this](Node* child) { OnNodeAdded(child); });
      break;
    case event::Node::ChangeType::Deleted:
      OnNodeRemoved(event.Origin);
      Node::ForEachChild(event.Origin,
                         [this](Node* child) { OnNodeRemoved(child); });
      break;
    case event::Node::ChangeType::ChildRemoved:
      OnNodeRemoved(event.ChangedNode);
      Node::ForEachChild(event.ChangedNode,
                         [this](Node* child) { OnNodeRemoved(child); });
      break;
    default:;  // do nothing
  }
}

void UpdateGraphComponent::OnNodeStateChanged(Node* node) {
  if (node->IsDirty()) {
    dirtyNodesPtr_->push_back(node);
  } else if (node->IsState(Node::State::Delete)) {
    for (auto itr = dirtyNodesPtr_->begin(); itr != dirtyNodesPtr_->end();
         ++itr) {
      if (*itr == node) {
        dirtyNodesPtr_->erase(itr);
        break;
      }
    }
  }
}

void UpdateGraphComponent::OnNodeAdded(Node* node) {
  if (node->IsDirty()) {
    dirtyNodesPtr_->push_back(node);
  }
}

void UpdateGraphComponent::OnNodeRemoved(Node* node) {
  for (auto itr = dirtyNodesPtr_->begin(); itr != dirtyNodesPtr_->end();
       ++itr) {
    if (*itr == node) {
      dirtyNodesPtr_->erase(itr);
      break;
    }
  }
}

Node* UpdateGraphComponent::computeTopDirtyNode(Node* node) {
  auto* mostTopDirtyNode = node;
  auto* current = node->GetParent();
  while (current != nullptr) {
    if (current->IsDirtyImpact(Node::DirtyImpact::Dependents) ||
        current->IsDirtyImpact(Node::DirtyImpact::Transform)) {
      mostTopDirtyNode = current;
    }
    current = current->GetParent();
  }
  return mostTopDirtyNode;
}
}  // namespace soil::stage::scene::component
