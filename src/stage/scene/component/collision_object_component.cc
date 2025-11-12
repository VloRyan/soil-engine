#include "stage/scene/component/collision_object_component.h"

#include <utility>

#include "stage/scene/node.h"
#include "stage/scene/world/world_node.h"

namespace soil::stage::scene::component {
CollisionObjectComponent::CollisionObjectComponent(
    soil::world::volume::Volume* volume,
    const soil::world::entity::CollisionObject::ContactType contactType,
    const std::vector<soil::world::entity::CollisionObject::ContactType>&
        contactTypes)
    : Component(Type::WorldEntity),
      object_(new soil::world::entity::CollisionObject(volume, contactType,
                                                       contactTypes)),
      world_(nullptr) {
  object_->SetOnWorldUpdate([this] {
    auto parent = GetParent();
    if (parent != nullptr) {
      parent->SetPosition(object_->GetPosition());
    }
  });
}

void CollisionObjectComponent::Update() {
  auto parent = GetParent();
  if (parent != nullptr && parent->GetPosition() != object_->GetPosition()) {
    object_->SetPosition(parent->GetPosition());
    if (world_ != nullptr) {
      world_->UpdatePosition(this);
    }
  }
  Component::Update();
}

void CollisionObjectComponent::SetParent(Node* parent) {
  if (GetParent() == parent) {
    return;
  }
  if (parent != nullptr) {
    object_->SetPosition(parent->GetPosition());
  }
  Component::SetParent(parent);
}

soil::world::entity::CollisionObject* CollisionObjectComponent::Object() const {
  return object_;
}

void CollisionObjectComponent::SetWorld(
    soil::stage::scene::world::WorldNode* world) {
  world_ = world;
}

soil::stage::scene::world::WorldNode* CollisionObjectComponent::GetWorld()
    const {
  return world_;
}
glm::vec3 CollisionObjectComponent::GetPosition() const {
  return Object()->GetPosition();
}
void CollisionObjectComponent::SetPosition(const glm::vec3& pos) {
  Object()->SetPosition(pos);
}
glm::vec3 CollisionObjectComponent::GetVelocity() const {
  return Object()->GetVelocity();
}
void CollisionObjectComponent::SetVelocity(const glm::vec3& velocity) {
  Object()->SetVelocity(velocity);
  if (world_ == nullptr) {
    return;
  }
  if (velocity != glm::vec3(0.F)) {
    world_->Activate(this);
  }
}

void CollisionObjectComponent::SetContactResponse(
    const std::function<
        void(soil::stage::scene::component::CollisionObjectComponent*,
             const soil::stage::scene::component::CollisionObjectComponent*)>&
        response) {
  contactResponse_ = response;
  object_->SetContactResponse(
      [this](soil::world::entity::CollisionObject* object,
             const soil::world::entity::CollisionObject* other) {
        auto otherComp = world_->ResolveCollisionObjectComponent(other);
        contactResponse_(this, otherComp);
      });
}

}  // namespace soil::stage::scene::component
