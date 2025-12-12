#include "stage/scene/component/world/world_component.h"

#include "stage/stage.h"
namespace soil::stage::scene::component::world {

WorldComponent::WorldComponent(soil::world::World *world)
    : component::Component(component::Component::Type::World),
      world_(world),
      linked_(false),
      hook_(
          {event::Hook::EventType::Node},
          {stage::hook::TriggerHook::TriggerPoint{
              .TriggerType = hook::TriggerHook::TriggerType::BeforeUpdateScene,
          }}) {
  if (world_ == nullptr) {
    throw std::invalid_argument("world shall not be NULL");
  }
  hook_.SetNodeEventCallback([this](const auto &event) { OnEvent(event); });
  hook_.SetTriggerCallback([this](auto &trigger) { OnTrigger(trigger); });
}

WorldComponent::~WorldComponent() { delete world_; }

void WorldComponent::SetParent(Node *parent) {
  Component::SetParent(parent);
  hook_.SetTriggerRoot(parent);
  if (parent == nullptr) {
    return;
  }
  std::vector<component::Component *> components;
  Node::ForEachChild(parent_, [this, &components](Node *child) {
    components.clear();
    child->GetComponents(components, component::Component::Type::WorldEntity);
    for (auto *comp : components) {
      auto *colObj = dynamic_cast<CollisionObjectComponent *>(comp);
      if (colObj == nullptr) {
        continue;
      }
      Insert(colObj);
    }
  });
}

void WorldComponent::OnEvent(const stage::event::Node &event) {
  if (event.ChangeType == stage::event::Node::ChangeType::Component &&
      event.ComponentEvent.Origin->GetType() ==
          component::Component::Type::WorldEntity) {
    Handle(event.ComponentEvent);
    return;
  }

  if (event.ChangeType == stage::event::Node::ChangeType::ChildRemoved) {
    RemoveAllDependentWorldComponents(event.ChangedNode);
    return;
  }
  if (event.ChangeType == stage::event::Node::ChangeType::Deleted) {
    RemoveAllDependentWorldComponents(event.Origin);
    return;
  }
}

void WorldComponent::Handle(const stage::event::Component &event) {
  switch (event.Trigger) {
    case stage::event::Component::TriggerType::Added: {
      if (!IsBelowMe(event.Origin->GetParent())) {
        return;
      }
      auto *colObj = dynamic_cast<CollisionObjectComponent *>(event.Origin);
      if (colObj == nullptr) {
        return;
      }
      Insert(colObj);
      colObj->SetWorld(this);
      break;
    }
    case stage::event::Component::TriggerType::Removed: {
      auto *colObj = dynamic_cast<CollisionObjectComponent *>(event.Origin);
      if (colObj == nullptr || colObj->GetWorld() != this) {
        return;
      }
      Remove(colObj);
      colObj->SetWorld(nullptr);
      break;
    }
    default:;
  }
}

void WorldComponent::RemoveAllDependentWorldComponents(Node *node) {
  const auto removeCollisionObjectComp = [this](auto *component) {
    auto *colObj = dynamic_cast<CollisionObjectComponent *>(component);
    if (colObj == nullptr || colObj->GetWorld() != this) {
      return;
    }
    Remove(colObj);
    colObj->SetWorld(nullptr);
  };
  node->ForEachComponent(removeCollisionObjectComp,
                         component::Component::Type::WorldEntity);
  Node::ForEachChild(node, [removeCollisionObjectComp](Node *child) {
    child->ForEachComponent(removeCollisionObjectComp,
                            component::Component::Type::WorldEntity);
  });
}

void WorldComponent::OnStageChanged(class Stage *stage,
                                    class Stage *prevStage) {
  if (stage == prevStage) {
    return;
  }
  Component::OnStageChanged(stage, prevStage);
  hook_.SetStage(stage);
  if (prevStage != nullptr) {
    Clear();
  }
  if (stage != nullptr) {
    std::vector<component::Component *> components;
    Node::ForEachChild(parent_, [this, &components](Node *child) {
      components.clear();
      child->GetComponents(components, component::Component::Type::WorldEntity);
      for (auto *comp : components) {
        auto *colObj = dynamic_cast<CollisionObjectComponent *>(comp);
        if (colObj == nullptr) {
          continue;
        }
        Insert(colObj);
      }
    });
  }
}
void WorldComponent::OnTrigger(
    const stage::hook::TriggerHook::TriggerPoint &point) {
  world_->Update();
}
void WorldComponent::Activate(CollisionObjectComponent *comp) {
  world_->Activate(comp->Object());
}

void WorldComponent::Insert(CollisionObjectComponent *comp) {
  world_->Insert(comp->Object());
  auto itr = objectToComponentMap_.find(comp->Object());
  if (itr != objectToComponentMap_.end()) {
    objectToComponentMap_.erase(itr);
  }
  objectToComponentMap_.insert({comp->Object(), comp});
}
void WorldComponent::Remove(CollisionObjectComponent *comp) {
  world_->Remove(comp->Object());
  auto itr = objectToComponentMap_.find(comp->Object());
  if (itr != objectToComponentMap_.end()) {
    objectToComponentMap_.erase(itr);
  }
}

void WorldComponent::UpdatePosition(CollisionObjectComponent *comp) {
  world_->Remove(comp->Object());
  world_->Insert(comp->Object());
}

bool WorldComponent::IsBelowMe(Node *node) const {
  auto *current = node->GetParent();
  while (current != nullptr) {
    if (current == parent_) {
      return true;
    }
    current = current->GetParent();
  }
  return false;
}

void WorldComponent::ForEachAt(
    const glm::vec3 point,
    const std::function<bool(const CollisionObjectComponent *objectComponent)>
        &f) const {
  std::vector<const soil::world::entity::CollisionObject *> objects;
  // TODO
  world_->QueryObjectsAt(point, objects);
  for (const auto *object : objects) {
    auto objectComp = ResolveCollisionObjectComponent(object);
    if (objectComp != nullptr) {
      if (const auto ret = f(objectComp); !ret) {
        return;
      }
    }
  }
}

void WorldComponent::ForEachInRange(
    const glm::vec3 point, const float radius,
    const std::function<bool(const CollisionObjectComponent *objectComponent)>
        &f) const {
  std::vector<const soil::world::entity::CollisionObject *> objects;

  world_->QueryObjectsInRange(point, radius, objects);
  for (const auto *object : objects) {
    auto objectComp = ResolveCollisionObjectComponent(object);
    if (objectComp != nullptr) {
      if (const auto ret = f(objectComp); !ret) {
        return;
      }
    }
  }
}
CollisionObjectComponent *WorldComponent::ResolveCollisionObjectComponent(
    const soil::world::entity::CollisionObject *object) const {
  auto itr = objectToComponentMap_.find(object);
  if (itr != objectToComponentMap_.end()) {
    return itr->second;
  }
  return nullptr;
}

void WorldComponent::Clear() {
  world_->Clear();
  objectToComponentMap_.clear();
}
const soil::world::volume::Container *WorldComponent::Container() const {
  return world_->Container();
}
}  // namespace soil::stage::scene::component::world
