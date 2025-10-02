
#include "stage/scene/scene.h"

#include <deque>

#include "stage/scene/node.h"
#include "stage/stage.h"
#include "util/deque.hpp"

namespace soil::stage::scene {
Scene::Scene()
    : Node(Type::Scene),
      stage_(nullptr),
      dirtyNodesPtr_(&dirtyNodes_),
      renderContainer_(new video::render::Container()),
      pipeline_(nullptr) {
  eventReceiverNodes_.resize(static_cast<int>(ReceiverType::COUNT));
}

Scene::~Scene() {
  if (const auto stage = GetStage(); stage != nullptr) {
    stage->RemoveScene(this);
  }
  activeUpdateNodes_.clear();
  eventReceiverNodes_.clear();
  ForEachChild(this, [this](Node* child) { child->RemoveListener(this); });
  delete renderContainer_;
}

void Scene::Render(video::render::State& state) {
  for (const auto hook : renderHooks_) {
    hook->OnRender(state);
  }
  if (pipeline_ != nullptr) {
    pipeline_->Run(state);
  }
}

void Scene::Update() {
  for (const auto* node : nodesToDelete_) {
    delete node;
  }
  nodesToDelete_.clear();
  for (auto* node : dirtyActiveUpdateNodes_) {
    if (node->GetUpdateType() == UpdateType::Active) {
      activeUpdateNodes_.push_back(node);
    } else {
      for (auto itr = activeUpdateNodes_.begin();
           itr != activeUpdateNodes_.end(); ++itr) {
        if (*itr == node) {
          activeUpdateNodes_.erase(itr);
          break;
        }
      }
    }
  }
  dirtyActiveUpdateNodes_.clear();
  for (auto* node : activeUpdateNodes_) {
    node->Update();
  }
  auto* lastDirtyNodes = dirtyNodesPtr_;
  if (dirtyNodesPtr_ == &dirtyNodes_) {
    dirtyNodesPtr_ = &dirtyNodes2_;
  } else {
    dirtyNodesPtr_ = &dirtyNodes_;
  }
  for (auto* node : *lastDirtyNodes) {
    if (!node->IsDirty()) {
      continue;
    }
    computeTopDirtyNode(node, this)->Update();
  }
  lastDirtyNodes->clear();
  for (const auto hook : updateHooks_) {
    hook->OnUpdate();
  }
}

void Scene::Handle(const event::Node& event) {
  switch (event.GetChangeType()) {
    case event::Node::ChangeType::State:
      OnNodeStateChanged(event.GetOrigin());
      break;
    case event::Node::ChangeType::ChildAdded:
      OnNodeAdded(event.GetChangedNode());
      ForEachChild(event.GetChangedNode(),
                   [this](Node* child) { OnNodeAdded(child); });
      break;
    case event::Node::ChangeType::Deleted:
      OnNodeRemoved(event.GetOrigin());
      ForEachChild(event.GetOrigin(),
                   [this](Node* child) { OnNodeRemoved(child); });
      break;
    case event::Node::ChangeType::ChildRemoved:
      OnNodeRemoved(event.GetChangedNode());
      ForEachChild(event.GetChangedNode(),
                   [this](Node* child) { OnNodeRemoved(child); });
      break;
    case event::Node::ChangeType::UpdateType:
      dirtyActiveUpdateNodes_.emplace_back(event.GetOrigin());
      break;
    case event::Node::ChangeType::Component:
      Handle(event.GetComponentEvent());
      break;
    default:;  // do nothing
  }
}

void Scene::OnNodeStateChanged(Node* node) {
  if (node->IsDirty() && node->GetUpdateType() != UpdateType::Active) {
    dirtyNodesPtr_->push_back(node);
  } else if (node->IsState(State::Delete)) {
    nodesToDelete_.push_back(node);
  }
}

void Scene::OnNodeAdded(Node* node) {
  node->AddListener(this);

  if (node->GetUpdateType() == UpdateType::Active) {
    dirtyActiveUpdateNodes_.push_back(node);
  } else {
    if (node->IsDirty()) {
      dirtyNodesPtr_->push_back(node);
    } else {
      node->SetDirty(DirtyImpact::Self);
    }
  }
  for (auto i = 1; i < static_cast<int>(ReceiverType::COUNT); ++i) {
    const auto type = static_cast<ReceiverType>(i);
    if (node->GetReceiverType(type)) {
      eventReceiverNodes_[i].push_back(node);
    }
  }
  if (!componentEventHandler_.empty()) {
    node->ForEachComponent([this](component::Component* component) {
      Handle(event::Component(component, event::Component::ChangeType::Added));
    });
  }
}

void Scene::RemoveChild(Node* node) {
  OnNodeRemoved(node);
  ForEachChild(node, [this](Node* child) { OnNodeRemoved(child); });
  Node::RemoveChild(node);
}

void Scene::OnNodeRemoved(Node* node) {
  if (node->GetUpdateType() == UpdateType::Active) {
    for (auto itr = dirtyActiveUpdateNodes_.begin();
         itr != dirtyActiveUpdateNodes_.end(); ++itr) {
      if (*itr == node) {
        dirtyActiveUpdateNodes_.erase(itr);
        break;
      }
    }
    for (auto itr = activeUpdateNodes_.begin(); itr != activeUpdateNodes_.end();
         ++itr) {
      if (*itr == node) {
        activeUpdateNodes_.erase(itr);
        break;
      }
    }
  }
  if (node->IsDirty()) {
    for (auto itr = dirtyNodesPtr_->begin(); itr != dirtyNodesPtr_->end();
         ++itr) {
      if (*itr == node) {
        dirtyNodesPtr_->erase(itr);
        break;
      }
    }
  }
  for (auto i = 1; i < static_cast<int>(ReceiverType::COUNT); ++i) {
    const auto type = static_cast<ReceiverType>(i);
    if (node->GetReceiverType(type)) {
      for (auto itr = eventReceiverNodes_[i].begin();
           itr != eventReceiverNodes_[i].end(); ++itr) {
        if (*itr == node) {
          eventReceiverNodes_[i].erase(itr);
          break;
        }
      }
    }
  }
  node->RemoveListener(this);
  if (!componentEventHandler_.empty()) {
    node->ForEachComponent([this](component::Component* component) {
      for (auto* listener : componentEventHandler_) {
        listener->Handle(
            event::Component(component, event::Component::ChangeType::Removed));
      }
    });
  }
}

void Scene::addChild(Node* node) {
  OnNodeAdded(node);
  ForEachChild(node, [this](Node* child) { OnNodeAdded(child); });
  Node::addChild(node);
}

void Scene::addHook(hook::Hook* hook) {
  switch (hook->GetType()) {
    case hook::Type::AfterUpdateScene: {
      auto* uHook = dynamic_cast<hook::UpdateHook*>(hook);
#ifdef DEBUG
      if (uHook == nullptr) {
        throw std::invalid_argument("hook can not be cast to UpdateHook");
      }
#endif
      updateHooks_.push_back(uHook);
      break;
    }
    case hook::Type::Render: {
      auto* rHook = dynamic_cast<hook::RenderHook*>(hook);
#ifdef DEBUG
      if (rHook == nullptr) {
        throw std::invalid_argument("hook can not be cast to RenderHook");
      }
#endif
      renderHooks_.push_back(rHook);
      break;
    }
  }
  if (hook->GetHandlerType() == hook::Hook::HandlerType::Component) {
    componentEventHandler_.push_back(hook);
  }
}

void Scene::RemoveHook(hook::Hook* theHook) {
  switch (theHook->GetType()) {
    case hook::Type::AfterUpdateScene:
      for (auto itr = updateHooks_.begin(); itr != updateHooks_.end(); ++itr) {
        if (*itr == theHook) {
          updateHooks_.erase(itr);
          break;
        }
      }
    case hook::Type::Render:
      for (auto itr = renderHooks_.begin(); itr != renderHooks_.end(); ++itr) {
        if (*itr == theHook) {
          renderHooks_.erase(itr);
          break;
        }
      }
  }
}

video::render::Container* Scene::GetRenderContainer() const {
  return renderContainer_;
}

void Scene::Handle(const event::Component& event) {
  for (auto* handler : componentEventHandler_) {
    handler->Handle(event);
  }
}

Node* Scene::computeTopDirtyNode(Node* node, const Node* except) {
  auto* topDirtyNode = node;
  auto* current = node;
  while (current != nullptr) {
    current = current->GetParent();
    if (current == except) {
      current = nullptr;
    } else {
      if (current != nullptr && current != except && current->IsDirty()) {
        topDirtyNode = current;
      }
    }
  }
  return topDirtyNode;
}

video::render::Pipeline* Scene::GetPipeline() const { return pipeline_; }

void Scene::SetPipeline(video::render::Pipeline* const pipeline) {
  pipeline_ = pipeline;
}

void Scene::SetStage(Stage* stage) {
  if (stage_ != nullptr) {
    stage_->RemoveScene(this);
    stage_->RemoveListener(this);
  }
  stage_ = stage;
  if (stage_ != nullptr) {
    stage_->AddListener(this);
  }
}

void Scene::Handle(const input::Event& event) {
  for (auto* const node :
       eventReceiverNodes_[static_cast<int>(ReceiverType::Input)]) {
    node->Handle(event);
  }
}

void Scene::Handle(const WindowEvent& event) {
  for (auto* const node :
       eventReceiverNodes_[static_cast<int>(ReceiverType::Window)]) {
    node->Handle(event);
  }
}

void Scene::Handle(const event::GameEvent& event) {}

Stage* Scene::GetStage() const { return stage_; }
}  // namespace soil::stage::scene
