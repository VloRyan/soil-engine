
#include "stage/scene/node.h"

#include <cstring>
#include <debug/deque>
#include <ranges>
#include <stdexcept>

#include "stage/event/node.h"
#include "stage/scene/component/transform_component.h"
#include "stage/stage.h"

namespace soil::stage::scene {
Node::Node(const Type type)
    : type_(type),
      parent_(nullptr),
      state_(State::Normal),
      updateType_(UpdateType::Passive),
      transform_(new component::TransformComponent()) {
  const auto compTypeIndex = static_cast<std::int8_t>(transform_->GetType());
  components_[compTypeIndex].push_back(transform_);
  transform_->SetParent(this);
  transform_->AddListener(this);
}

Node::~Node() {
  Node::fire(event::Node::MakeNodeDeletedEvent(this));
  for (auto* child : children_) {
    child->SetParent(nullptr);  // prevent child events
    delete child;
  }
  children_.clear();
  transform_->RemoveListener(this);
  for (const auto& comps : components_ | std::views::values) {
    for (auto* comp : comps) {
      comp->SetParent(nullptr);
      delete comp;
    }
  }
  components_.clear();
  if (auto* parent = GetParent(); parent != nullptr) {
    for (auto itr = parent->children_.begin(); itr != parent->children_.end();
         ++itr) {
      if (*itr != this) {
        continue;
      }
      parent->children_.erase(itr);
      break;
    }
  }
}

void Node::SetParent(Node* parent) {
  if (parent_ == parent) {
    return;
  }
  auto* prevScene = Root();
  auto* prevStage = prevScene != nullptr ? prevScene->Stage() : nullptr;
  class Scene* scene = nullptr;
  class Stage* stage = nullptr;
  parent_ = parent;
  if (parent_ != nullptr) {
    transform_->UpdateTransform(parent_->transform_->GetMatrix());
    stage = Stage();
  }
  if (prevStage != stage) {
    OnStageChanged(stage, prevStage);
  }
}

void Node::OnStageChanged(class Stage* stage, class Stage* prevStage) {
  ForEachChild(this, [stage, prevStage](Node* node) {
    node->OnStageChanged(stage, prevStage);
  });
  ForEachComponent([stage, prevStage](component::Component* component) {
    component->OnStageChanged(stage, prevStage);
  });
}

void Node::addChild(Node* node) {
  if (node->GetParent() == this) {
    return;
  }
  node->SetParent(this);
  children_.push_back(node);
  fire(event::Node::MakeChildAddedEvent(this, node));
}

void Node::RemoveChild(Node* node) {
  for (auto itr = children_.begin(); itr != children_.end(); ++itr) {
    if (*itr != node) {
      continue;
    }
    fire(event::Node::MakeChildRemovedEvent(this, node));
    node->SetParent(nullptr);
    children_.erase(itr);
    break;
  }
}

void Node::SetUpdateType(const UpdateType type) {
  if (type == updateType_) {
    return;
  }
  updateType_ = type;
  fire(event::Node(this, event::Node::ChangeType::UpdateType));
}

Node::UpdateType Node::GetUpdateType() const { return updateType_; }

Node::Type Node::GetType() const { return type_; }

glm::vec3 Node::GetPosition() const { return transform_->GetPosition(); }

void Node::SetPosition(const glm::vec3& pos) { transform_->SetPosition(pos); }

glm::vec3 Node::GetLocalPosition() const {
  return transform_->GetLocalPosition();
}
void Node::SetLocalPosition(const glm::vec3& pos) {
  transform_->SetLocalPosition(pos);
}

/*
void Node::SetDirection(const glm::vec3& direction) {
  auto localDirection = glm::vec3(localTransform_[2]);
  /*
  if (auto* parent = GetParent(); parent != nullptr) {
    auto relDir = direction - parent->GetDirection();
    if (relDir == localDirection) {
      return;
    }
    localTransform_[2] = glm::vec4(relDir, 0.F);
    auto localPos = glm::vec3(localTransform_[3]);
    localTransform_ = glm::inverse(parent->GetTransform()) * localTransform_;
    localTransform_[3] = glm::vec4(localPos, 1.F);
  } else {*//*
  // TODO
  if (direction == localDirection) {
    return;
  }
  localTransform_[2] = glm::vec4(direction, 0.F);
  Object3d::SetTransform(localTransform_);
  //}
  SetDirty(DirtyImpact::Transform);
}
/*
void Node::SetRight(const glm::vec3& right) {
  auto localRight = glm::vec3(localTransform_[0]);
  /*if (auto* parent = GetParent(); parent != nullptr) {
    auto relRight = right - parent->GetRight();
    if (relRight == localRight) {
      return;
    }
    localTransform_[0] = glm::vec4(relRight, 1.F);
  } else {*//*
  // TODO
  if (right == localRight) {
    return;
  }
  localTransform_[0] = glm::vec4(right, 1.F);
  Object3d::SetTransform(localTransform_);
  //}
  SetDirty(DirtyImpact::Transform);
}
/*
void Node::SetUp(const glm::vec3& up) {
  auto localUp = glm::vec3(localTransform_[1]);
  /*if (auto* parent = GetParent(); parent != nullptr) {
    auto relUp = up - parent->GetUp();
    if (relUp == localUp) {
      return;
    }
    localTransform_[1] = glm::vec4(relUp, 1.F);
  } else {*//*
  // TODO
  if (up == localUp) {
    return;
  }
  localTransform_[0] = glm::vec4(up, 1.F);
  Object3d::SetTransform(localTransform_);
  //}
  SetDirty(DirtyImpact::Transform);
}
/*
void Node::SetTransform(const glm::mat4& transform) {
  if (transform == localTransform_) {
    return;
  }
  localTransform_ = transform;
  SetDirty(DirtyImpact::Transform);
}
*/

void Node::Update() {
  if (IsDirty()) {
    UpdateDirty();
  } else {
    for (auto* comp : alwaysUpdateComponents_) {
      comp->Update();
    }
  }
}

component::TransformComponent& Node::Transform() const { return *transform_; }

Node* Node::Root() const {
  auto p = const_cast<Node*>(this);
  while (p->GetParent() != nullptr) {
    p = p->GetParent();
  }
  return p;
}

void Node::UpdateDirty() {
  if (IsDirtyImpact(DirtyImpact::Transform)) {
    if (GetParent() != nullptr) {
      transform_->UpdateTransform(GetParent()->transform_->GetMatrix());
    }
    ForEachComponent([this](component::Component* component) {
      if (component == transform_) {
        return;
      }
      component->Update();
    });
    for (auto* child : children_) {
      child->MarkDirtyWith(DirtyImpact::Transform);
      child->UpdateDirty();
    }
  } else {
    if (IsDirtyImpact(DirtyImpact::Dependents)) {
      ForEachComponent(
          [](component::Component* component) { component->Update(); });
      for (auto* child : children_) {
        child->MarkDirtyWith(DirtyImpact::Dependents);
        child->UpdateDirty();
      }
    }
  }

  incorporateAddedComponents();
  SetState(State::Normal);
  dirtyImpacts_ = 0;
}

void Node::incorporateAddedComponents() {
  for (auto* comp : addedComponents_) {
    if (comp->GetParent() != nullptr) {
      comp->GetParent()->RemoveComponent(comp);
    }
    comp->SetParent(this);
    comp->AddListener(this);
    comp->Update();
    const auto compTypeIndex = static_cast<std::int8_t>(comp->GetType());

    components_[compTypeIndex].push_back(comp);
    if (comp->GetUpdateType() == component::Component::UpdateType::Always) {
      alwaysUpdateComponents_.push_back(comp);
    }
    const auto addedEvent =
        event::Component(comp, event::Component::TriggerType::Added);
    fire(event::Node::MakeComponentEvent(this, addedEvent));
  }
  addedComponents_.clear();
}

bool Node::IsDirty() const { return state_ == State::Dirty; }

Node::State Node::GetState() const { return state_; }

bool Node::IsState(const State state) const { return state_ == state; }

bool Node::IsDirtyImpact(const DirtyImpact cause) const {
  return IsDirtyImpact(dirtyImpacts_, cause);
}

void Node::SetState(const State state) {
  if (state_ == state || state_ == State::Delete) {
    return;
  }
  state_ = state;
  fire(event::Node(this, event::Node::ChangeType::State));
}

void Node::SetDirty(DirtyImpact cause) {
  dirtyImpacts_[static_cast<std::int8_t>(cause)] = true;
  SetState(State::Dirty);
}
void Node::MarkDirtyWith(DirtyImpact cause) {
  dirtyImpacts_[static_cast<std::int8_t>(cause)] = true;
  state_ = State::Dirty;
}

Node* Node::GetParent() const { return parent_; }

const std::vector<Node*>& Node::GetChildren() const { return children_; }

void Node::addComponent(component::Component* comp) {
#ifdef DEBUG
  if (comp == nullptr) {
    throw std::invalid_argument("component cannot be null");
  }
#endif
  if (this == comp->GetParent()) {
    return;
  }
  addedComponents_.push_back(comp);
  SetDirty(DirtyImpact::Components);
}

void Node::RemoveComponent(component::Component* comp) {
  if (this != comp->GetParent()) {
    for (auto itr = addedComponents_.begin(); itr != addedComponents_.end();
         ++itr) {
      if (comp == *itr) {
        addedComponents_.erase(itr);
        return;
      }
    }
    return;
  }
  const auto typeIndex = static_cast<std::int8_t>(comp->GetType());

  for (auto itr = components_[typeIndex].begin();
       itr != components_[typeIndex].end(); ++itr) {
    if (comp == *itr) {
      components_[typeIndex].erase(itr);
      comp->SetParent(nullptr);
      comp->RemoveListener(this);
      break;
    }
  }
  if (comp->GetUpdateType() == component::Component::UpdateType::Always) {
    for (auto itr = alwaysUpdateComponents_.begin();
         itr != alwaysUpdateComponents_.end(); ++itr) {
      if (comp == *itr) {
        alwaysUpdateComponents_.erase(itr);
        break;
      }
    }
  }
  const auto removedEvent =
      event::Component(comp, event::Component::TriggerType::Removed);
  fire(event::Node::MakeComponentEvent(this, removedEvent));
}

bool Node::HasComponent(component::Component::Type type) const {
  return components_.contains(static_cast<std::int8_t>(type));
}

void Node::Handle(const event::Component& event) {
  if (event.Origin->GetParent() != this) {
    return;
  }
  switch (event.Trigger) {
    case event::Component::TriggerType::Changed: {
      switch (event.Changed) {
        case event::Component::ChangeType::Data:
          for (const auto* justAddedComp : addedComponents_) {
            if (event.Origin == justAddedComp) {
              return;
            }
          }
          if (event.Origin == transform_) {
            // TODO: Why is Dependents not enough?
            SetDirty(DirtyImpact::Transform);
          }
          break;
        case event::Component::ChangeType::UpdateType:
          if (event.Origin->GetUpdateType() ==
              component::Component::UpdateType::Always) {
            alwaysUpdateComponents_.push_back(event.Origin);
          } else {
            for (auto itr = alwaysUpdateComponents_.begin();
                 itr != alwaysUpdateComponents_.end(); ++itr) {
              if (event.Origin == *itr) {
                alwaysUpdateComponents_.erase(itr);
                break;
              }
            }
          }
          break;
        default:;
      }
      break;
    }
    default:;
  }
  fire(event::Node::MakeComponentEvent(this, event));
}

bool Node::IsReceiverOf(ReceiverType type) const {
  if (type == ReceiverType::None) {
    return receiveTypeFlags_ == 0;
  }
  return receiveTypeFlags_[static_cast<std::int8_t>(type) - 1];
}

void Node::SetReceiverType(ReceiverType type, const bool value) {
  if (type == ReceiverType::None) {
    receiveTypeFlags_ = 0;
    return;
  }
  receiveTypeFlags_[static_cast<std::int8_t>(type) - 1] = value;
}

std::bitset<4> Node::GetDirtyImpacts() const { return dirtyImpacts_; }

void Node::ForEachComponent(
    const std::function<void(component::Component*)>& func,
    component::Component::Type ofType) const {
  if (ofType == component::Component::Type::Any) {
    for (const auto& comps : components_ | std::views::values) {
      for (auto* comp : comps) {
        func(comp);
      }
    }
  } else {
    const auto typeIndex = static_cast<std::int8_t>(ofType);
    const auto pos = components_.find(typeIndex);
    if (pos == components_.end() || pos->second.empty()) {
      return;
    }
    for (auto* comp : pos->second) {
      func(comp);
    }
  }
}

void Node::GetComponents(std::vector<component::Component*>& comps,
                         const component::Component::Type type) const {
  if (type == component::Component::Type::Any) {
    for (std::int8_t key = 0;
         key <= static_cast<std::int8_t>(component::Component::Type::World);
         key++) {
      const auto itr = components_.find(key);
      if (itr == components_.end() || itr->second.empty()) {
        continue;
      }
      comps.insert(comps.end(), itr->second.begin(), itr->second.end());
    }
    return;
  }
  const auto typeIndex = static_cast<std::int8_t>(type);
  const auto itr = components_.find(typeIndex);
  if (itr == components_.end() || itr->second.empty()) {
    return;
  }
  comps.insert(comps.end(), itr->second.begin(), itr->second.end());
}

component::Component* Node::GetFirstComponent(
    component::Component::Type type) const {
  auto itr = components_.end();
  if (type == component::Component::Type::Any) {
    itr = components_.begin();
  } else {
    const auto typeIndex = static_cast<std::int8_t>(type);
    itr = components_.find(typeIndex);
  }
  if (itr == components_.end() || itr->second.empty()) {
    return nullptr;
  }
  return itr->second.front();
}

bool Node::IsDirtyImpact(const std::bitset<4>& dirtyImpacts,
                         DirtyImpact impact) {
  return dirtyImpacts[static_cast<std::int8_t>(impact)];
}

void Node::ForEachChild(const Node* node,
                        const std::function<void(Node* child)>& func) {
  if (node->GetChildren().empty()) {
    return;
  }
  constexpr auto defaultBufferSize = 8;
  auto bufferSize =
      std::max(defaultBufferSize, static_cast<int>(node->GetChildren().size()));
  auto buffer = new Node*[bufferSize];
  std::memcpy(buffer, &node->GetChildren().front(),
              node->GetChildren().size() * sizeof(Node*));
  auto cursor = 0;
  auto dataEnd = node->GetChildren().size();
  auto freeCapacity = bufferSize - dataEnd;
  while (cursor < dataEnd) {
    func(buffer[cursor]);
    auto& c = buffer[cursor]->GetChildren();
    const auto elemsToPush = c.size();
    if (elemsToPush == 0) {
      cursor++;
      continue;
    }
    if (freeCapacity < elemsToPush) {
      if (cursor + 1 < dataEnd) {
        std::memcpy(buffer, &buffer[cursor + 1],
                    (dataEnd - 1 - cursor) * sizeof(Node*));
        dataEnd -= cursor + 1;
        cursor = -1;
        freeCapacity = bufferSize - dataEnd;
      } else {
        dataEnd = 0;
        cursor = -1;
        freeCapacity = bufferSize;
      }
      if (freeCapacity < elemsToPush) {
        bufferSize += static_cast<int>(elemsToPush);
        const auto newBuffer = new Node*[bufferSize];
        memcpy(newBuffer, buffer, dataEnd * sizeof(Node*));
        delete[] buffer;
        buffer = newBuffer;
        freeCapacity = bufferSize - dataEnd;
      }
    }
    std::memcpy(buffer + dataEnd, &c.front(), elemsToPush * sizeof(Node*));
    dataEnd += elemsToPush;
    freeCapacity -= elemsToPush;
    cursor++;
  }
  delete[] buffer;
}

void Node::fire(const event::Node& event) const {
  Observable::fire(event);
  auto* stage = Stage();
  if (stage != nullptr) {
    stage->Handle(event);
  }
}

/*void Node::Render(video::render::State& state) {
  ForEachComponent(
      [&state](component::Component* c) {
        auto* rc = dynamic_cast<component::render::RenderComponent*>(c);
        rc->Render(state);
      },
      component::Component::Type::Render);
}*/

Stage* Node::Stage() const {
  if (parent_ == nullptr) {
    return nullptr;
  }
  return parent_->Stage();
}
}  // namespace soil::stage::scene
