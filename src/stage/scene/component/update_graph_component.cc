#include "stage/scene/component/update_graph_component.h"

#include "stage/scene/node.h"
#include "stage/stage.h"

namespace soil::stage::scene::component {
UpdateGraphComponent::UpdateGraphComponent()
    : EventComponent(Type::Transform,
                     {.Events = {EventComponent::EventType::Node},
                      .TriggerPoints =
                          {
                              {.TriggerType = TriggerType::BeforeUpdateScene},
                          }}),
      dirtyNodesPtr_{&dirtyNodesFront_} {}

void UpdateGraphComponent::Update() {
  for (const auto* node : nodesToDelete_) {
    delete node;
  }
  nodesToDelete_.clear();
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
void UpdateGraphComponent::OnTrigger(
    const hook::TriggerHook::TriggerPoint& point) {
  Update();
}

void UpdateGraphComponent::OnEvent(const stage::event::Node& event) {
  switch (event.ChangeType) {
    case stage::event::Node::ChangeType::State:
      OnNodeStateChanged(event.Origin);
      break;
    case stage::event::Node::ChangeType::ChildAdded:
      OnNodeAdded(event.ChangedNode);
      Node::ForEachChild(event.ChangedNode,
                         [this](Node* child) { OnNodeAdded(child); });
      break;
    case stage::event::Node::ChangeType::Deleted:
      OnNodeRemoved(event.Origin);
      Node::ForEachChild(event.Origin,
                         [this](Node* child) { OnNodeRemoved(child); });
      break;
    case stage::event::Node::ChangeType::ChildRemoved:
      OnNodeRemoved(event.ChangedNode);
      Node::ForEachChild(event.ChangedNode,
                         [this](Node* child) { OnNodeRemoved(child); });
      break;
    default:;  // do nothing
  }
}

void UpdateGraphComponent::OnNodeStateChanged(Node* node) {
  if (node == nullptr) {
    return;
  }
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
    nodesToDelete_.push_back(node);
  }
}

void UpdateGraphComponent::OnNodeAdded(Node* node) {
  if (node != nullptr && node->IsDirty()) {
    dirtyNodesPtr_->push_back(node);
  }
}

void UpdateGraphComponent::OnNodeRemoved(Node* node) {
  if (node == nullptr) {
    return;
  }
  for (auto itr = dirtyNodesPtr_->begin(); itr != dirtyNodesPtr_->end();
       ++itr) {
    if (*itr == node) {
      dirtyNodesPtr_->erase(itr);
      break;
    }
  }
  for (auto itr = nodesToDelete_.begin(); itr != nodesToDelete_.end(); ++itr) {
    if (*itr == node) {
      nodesToDelete_.erase(itr);
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
