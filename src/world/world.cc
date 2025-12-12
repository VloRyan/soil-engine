#include "world/world.h"

#include <stdexcept>

#include "world/volume/quad_tree.h"
namespace soil::world {

World::World(const World::Definition& definition, volume::Container* container)
    : friction_(definition.Friction), container_(container), maxIdleTime_(10) {}

void World::Insert(entity::CollisionObject* object) {
  container_->Insert(object);
  if (object->GetVelocity() != glm::vec3(0.F)) {
    Activate(object);
  }
}

void World::Remove(entity::CollisionObject* object) {
  for (auto itr = activeObjects_.begin(); itr != activeObjects_.end(); ++itr) {
    if (itr->Object == object) {
      activeObjects_.erase(itr);
      break;
    }
  }
  container_->Remove(object);
}

void World::Activate(entity::CollisionObject* object) {
  // TODO: What if object is not part of this world?
  object->SetState(entity::CollisionObject::State::Active);
  for (auto other : activeObjects_) {
    if (other.Object == object) {
      other.IdleTime = 0;
      return;  // already active; reset time
    }
  }
  activeObjects_.emplace_back(object, 0);
}

void World::Update() {
  for (auto itr = activeObjects_.begin(); itr != activeObjects_.end();) {
    auto* object = itr->Object;
    auto velocity = object->GetVelocity();
    if (velocity == glm::vec3(0.F)) {
      itr->IdleTime++;
      if (itr->IdleTime >= maxIdleTime_) {
        itr->Object->SetState(entity::CollisionObject::State::Inactive);
        itr = activeObjects_.erase(itr);
      } else {
        itr->Object->SetState(entity::CollisionObject::State::Idle);
        ++itr;
      }
      continue;
    }
    itr->Object->SetState(entity::CollisionObject::State::Active);
    glm::vec3 newWorldPos = object->GetPosition() + velocity;
    std::vector<const entity::CollisionObject*> objects;
    container_->QueryObjectsAt(newWorldPos, objects);
    for (auto* other : objects) {
      if (object != other && object->CanCollideWith(other->GetContactType())) {
        object->HandleCollision(other);
      }
    }
    velocity = object->GetVelocity();
    if (velocity != glm::vec3(0.F)) {
      newWorldPos = object->GetPosition() + velocity;
      for (auto i = 0; i < 3; i++) {
        if (velocity[i] > 0.F) {
          velocity[i] = std::max(velocity[i] - friction_, 0.F);
        }
        if (velocity[i] < 0.F) {
          velocity[i] = std::min(velocity[i] + friction_, 0.F);
        }
      }
      object->Update(newWorldPos, velocity);
    }
    ++itr;
  }
}

void World::QueryObjectsAt(
    const glm::vec3& point,
    std::vector<const entity::CollisionObject*>& objects) const {
  return container_->QueryObjectsAt(point, objects);
}

void World::QueryObjectsInRange(
    const glm::vec3& point, float radius,
    std::vector<const entity::CollisionObject*>& objects) const {
  return container_->QueryObjectsInRange(point, radius, objects);
}
void World::Clear() {
  container_->Clear();
  activeObjects_.clear();
}
int World::GetMaxIdleTime() const { return maxIdleTime_; }
void World::SetMaxIdleTime(int maxIdleTime) { maxIdleTime_ = maxIdleTime; }
const soil::world::volume::Container* World::Container() { return container_; }
}  // namespace soil::world
