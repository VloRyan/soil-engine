#include "stage/scene/world/world_node.h"

#include "stage/stage.h"
namespace soil::stage::scene::world {

WorldNode::WorldNode(soil::world::World *world)
    : scene::Node(Node::Type::World), world_(world), linked_(false) {
  if (world_ == nullptr) {
    throw std::invalid_argument("world shall not be NULL");
  }
}

WorldNode::~WorldNode() { delete world_; }

void WorldNode::OnEvent(const event::Node &event) {
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

void WorldNode::Handle(const event::Component &event) {
  switch (event.Trigger) {
    case event::Component::TriggerType::Added: {
      if (!IsBelowMe(event.Origin->GetParent())) {
        return;
      }
      auto *colObj =
          dynamic_cast<component::CollisionObjectComponent *>(event.Origin);
      if (colObj == nullptr) {
        return;
      }
      Insert(colObj);
      colObj->SetWorld(this);
      break;
    }
    case event::Component::TriggerType::Removed: {
      auto *colObj =
          dynamic_cast<component::CollisionObjectComponent *>(event.Origin);
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

void WorldNode::RemoveAllDependentWorldComponents(Node *node) {
  const auto removeCollisionObjectComp = [this](auto *component) {
    auto *colObj =
        dynamic_cast<component::CollisionObjectComponent *>(component);
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

void WorldNode::OnStageChanged(class Stage *stage, class Stage *prevStage) {
  Node::OnStageChanged(stage, prevStage);
  if (stage == prevStage) {
    return;
  }
  if (prevStage != nullptr) {
    prevStage->RemoveEventHook(Root(), this);
    prevStage->RemoveTriggerHook(
        this,
        {.Root = Root(),
         .TriggerType = hook::TriggerHook::TriggerType::BeforeUpdateScene});
    Clear();
  }
  if (stage != nullptr) {
    stage->AddEventHook(Root(), this);
    stage->AddTriggerHook(
        this,
        {.Root = Root(),
         .TriggerType = hook::TriggerHook::TriggerType::BeforeUpdateScene});
    std::vector<component::Component *> components;
    ForEachChild(this, [this, &components](Node *child) {
      components.clear();
      child->GetComponents(components, component::Component::Type::WorldEntity);
      for (auto *comp : components) {
        auto *colObj =
            dynamic_cast<component::CollisionObjectComponent *>(comp);
        if (colObj == nullptr) {
          continue;
        }
        Insert(colObj);
      }
    });
  }
}
void WorldNode::OnTrigger(const TriggerPoint &point) {
  /*if (!linked_) {
    auto scene = ClimbUpToScene();
    if (scene != nullptr) {
      LinkToScene(*scene);
    }
  }*/
  world_->Update();
}
void WorldNode::Activate(component::CollisionObjectComponent *comp) {
  world_->Activate(comp->Object());
}

void WorldNode::Insert(component::CollisionObjectComponent *comp) {
  world_->Insert(comp->Object());
  auto itr = objectToComponentMap_.find(comp->Object());
  if (itr != objectToComponentMap_.end()) {
    objectToComponentMap_.erase(itr);
  }
  objectToComponentMap_.insert({comp->Object(), comp});
}
void WorldNode::Remove(component::CollisionObjectComponent *comp) {
  world_->Remove(comp->Object());
  auto itr = objectToComponentMap_.find(comp->Object());
  if (itr != objectToComponentMap_.end()) {
    objectToComponentMap_.erase(itr);
  }
}

void WorldNode::UpdatePosition(component::CollisionObjectComponent *comp) {
  world_->Remove(comp->Object());
  world_->Insert(comp->Object());
}

bool WorldNode::IsBelowMe(Node *node) const {
  auto *current = node->GetParent();
  while (current != nullptr) {
    if (current == this) {
      return true;
    }
    current = current->GetParent();
  }
  return false;
}

void WorldNode::ForEachAt(
    const glm::vec3 point,
    const std::function<bool(
        const component::CollisionObjectComponent *objectComponent)> &f) const {
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

void WorldNode::ForEachInRange(
    const glm::vec3 point, const float radius,
    const std::function<bool(
        const component::CollisionObjectComponent *objectComponent)> &f) const {
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
component::CollisionObjectComponent *WorldNode::ResolveCollisionObjectComponent(
    const soil::world::entity::CollisionObject *object) const {
  auto itr = objectToComponentMap_.find(object);
  if (itr != objectToComponentMap_.end()) {
    return itr->second;
  }
  return nullptr;
}

void WorldNode::Clear() {
  world_->Clear();
  objectToComponentMap_.clear();
}
const soil::world::volume::Container *WorldNode::Container() const {
  return world_->Container();
}
}  // namespace soil::stage::scene::world
