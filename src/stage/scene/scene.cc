
#include "stage/scene/scene.h"

#include <stdexcept>

#include "stage/stage.h"

namespace stage::scene {
constexpr auto SCENE_NODE_OFFSET = 100000;

Scene::Scene(Stage *stage)
    : Node(stage->AddScene(this), Type::Scene, {}, ".::SCENE::."),
      stage_(stage), dirtyNodesPtr_(&dirtyNodes_),
      nextId_(GetId() * SCENE_NODE_OFFSET), worldTransform_(1) {
  SetScene(this);
}

void Scene::RemoveChild(Node *node) {
  NodeRemoved(node);
  Node::RemoveChild(node);
}

void Scene::Render(video::render::State &state) {
  for (auto *viewer : viewer_) {
    if (!viewer->IsState(State::Normal)) {
      continue;
    }
    viewer->Render(state);
  }
}

void Scene::Update() {
  for (auto *node : activeUpdateNodes_) {
    node->Update();
  }
  auto *lastDirtyNodes = dirtyNodesPtr_;
  if (dirtyNodesPtr_ == &dirtyNodes_) {
    dirtyNodesPtr_ = &dirtyNodes2_;
  } else {
    dirtyNodesPtr_ = &dirtyNodes_;
  }
  for (auto *node : *lastDirtyNodes) {
    if (!node->IsDirty() || node->GetType() == Type::Viewer ||
        node->GetUpdateType() == UpdateType::Active) {
      continue;
    }
    if (auto *topDirty = computeTopDirtyNode(node); topDirty != nullptr) {
      topDirty->Update();
    }
  }
  lastDirtyNodes->clear();

  for (auto *viewer : viewer_) {
    viewer->Update();
  }
}

void Scene::Handle(const event::Node &event) {
  switch (event.GetChangeType()) {
  case event::Node::ChangeType::State:
    NodeStateChanged(event.GetOrigin());
    break;
  case event::Node::ChangeType::Created:
    NodeAdded(event.GetOrigin());
    break;
  case event::Node::ChangeType::ChildAdded:
    NodeAdded(event.GetChangedNode());
    break;
  case event::Node::ChangeType::Deleted:
    NodeRemoved(event.GetOrigin());
    break;
  case event::Node::ChangeType::ChildRemoved:
    NodeRemoved(event.GetChangedNode());
    break;
  default:; // do nothing
  }
}

void Scene::NodeStateChanged(Node *node) {
  if (node->IsDirty()) {
    if (node->IsDirtyCause(DirtyCauses::UpdateType)) {
      switch (node->GetUpdateType()) {
      case UpdateType::Active:
        activeUpdateNodes_.emplace_back(node);
        break;
      case UpdateType::Passive:
        for (auto itr = activeUpdateNodes_.begin();
             itr != activeUpdateNodes_.end(); ++itr) {
          if (*itr == node) {
            activeUpdateNodes_.erase(itr);
            break;
          }
        }
        break;
      }
    } else {
      dirtyNodesPtr_->push_back(node);
    }
  } else if (node->GetState() == State::Delete) {
    delete node;
  }
}

void Scene::NodeAdded(Node *node) {
  switch (node->GetType()) {
  case Type::Viewer: {
    auto *viewer = dynamic_cast<viewer::Node *>(node);
    if (viewer == nullptr) {
      return;
    }
    viewer_.push_back(viewer);
    if (viewer_.size() == 1) {
      forEachChild(this, [viewer](const Node *child) {
        for (const auto renderables =
                 child->GetComponents(component::Component::Type::Renderable);
             auto *const renderable : renderables) {
          viewer->AddRenderable(
              dynamic_cast<component::RenderableInstance *>(renderable));
        }
      });
    }
    break;
  }
  default:
    if (node->GetUpdateType() == UpdateType::Active) {
      activeUpdateNodes_.push_back(node);
    }
  }
  dirtyNodesPtr_->push_back(node);
  if (node->GetReceiveTypes()[static_cast<short>(ReceiverType::Window)]) {
    windowEventReceiverNodes_.push_back(node);
  }
  if (node->GetReceiveTypes()[static_cast<short>(ReceiverType::Input)]) {
    inputEventReceiverNodes_.push_back(node);
  }
}

void Scene::NodeRemoved(const Node *node) {
  switch (node->GetType()) {
  case Type::Viewer: {
    for (auto itr = viewer_.begin(); itr != viewer_.end(); ++itr) {
      if (*itr == node) {
        viewer_.erase(itr);
        break;
      }
    }
    break;
  }
  default:
    if (node->GetUpdateType() == UpdateType::Active) {
      for (auto itr = activeUpdateNodes_.begin();
           itr != activeUpdateNodes_.end(); ++itr) {
        if (*itr == node) {
          activeUpdateNodes_.erase(itr);
          break;
        }
      }
    }
  }
  if (node->GetReceiveTypes()[static_cast<short>(ReceiverType::Window)]) {
    for (auto itr = windowEventReceiverNodes_.begin();
         itr != windowEventReceiverNodes_.end(); ++itr) {
      if (*itr == node) {
        windowEventReceiverNodes_.erase(itr);
        break;
      }
    }
  }
  if (node->GetReceiveTypes()[static_cast<short>(ReceiverType::Input)]) {
    for (auto itr = inputEventReceiverNodes_.begin();
         itr != inputEventReceiverNodes_.end(); ++itr) {
      if (*itr == node) {
        inputEventReceiverNodes_.erase(itr);
        break;
      }
    }
  }
}

void Scene::Handle(const event::Component &event) {
  switch (event.GetChangeType()) {
  case event::Component::ChangeType::State:
    ComponentStateChanged(event.GetOrigin());
    break;
  case event::Component::ChangeType::Added:
    ComponentAdded(event.GetOrigin());
    break;
  case event::Component::ChangeType::Removed:
    ComponentRemoved(event.GetOrigin());
    break;
  }
}

void Scene::ComponentStateChanged(component::Component *component) {
  if (component->GetState() != component::Component::State::Normal) {
    return;
  }
  switch (component->GetType()) {
  case component::Component::Type::Renderable: {
    auto *rComp = dynamic_cast<component::RenderableInstance *>(component);
    for (auto *viewer : viewer_) {
      viewer->AddChangedRenderable(rComp);
    }
    break;
  }
  default:; // do nothing
  }
}

void Scene::ComponentAdded(component::Component *component) {
  switch (component->GetType()) {
  case component::Component::Type::Renderable: {
    auto *rComp = dynamic_cast<component::RenderableInstance *>(component);
    for (auto *viewer : viewer_) {
      viewer->AddRenderable(rComp);
    }
    break;
  }
  default:; // do nothing
  }
}

void Scene::ComponentRemoved(component::Component *component) {
  switch (component->GetType()) {
  case component::Component::Type::Renderable: {
    auto *rComp = dynamic_cast<component::RenderableInstance *>(component);
    for (auto *viewer : viewer_) {
      viewer->RemoveRenderable(rComp);
    }
    break;
  }
  default:;
  }
}

Node *Scene::computeTopDirtyNode(Node *current) {
  if (current->GetParent() != nullptr && current->GetParent()->IsDirty()) {
    return computeTopDirtyNode(current->GetParent());
  }
  return current;
}

void Scene::forEachChild(const Node *node,
                         const std::function<void(Node *child)> &func) {
  for (auto *const child : node->GetChildren()) {
    func(child);
    forEachChild(child, func);
  }
}

void Scene::Handle(const input::Event &event) {
  for (auto *const node : inputEventReceiverNodes_) {
    node->Handle(event);
  }
}

void Scene::Handle(const engine::WindowEvent &event) {
  for (auto *const node : windowEventReceiverNodes_) {
    node->Handle(event);
  }
}

const std::vector<viewer::Node *> &Scene::GetViewer() const { return viewer_; }

int Scene::GetNextId() {
  const auto nextId = ++nextId_;
  if (nextId % SCENE_NODE_OFFSET == 0) {
    throw std::runtime_error("nextId reached next scene range");
  }
  return nextId;
}

void Scene::AddChild(Node *child) { Node::AddChild(child); }

Resources &Scene::GetResources() const { return stage_->GetResources(); }

Stage *Scene::GetStage() const { return stage_; }
} // namespace stage::scene
