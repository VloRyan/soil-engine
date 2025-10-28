#ifndef SOIL_WORLD_WORLD_H
#define SOIL_WORLD_WORLD_H
#include "vector"
#include "world/entity/collision_object.h"
#include "world/volume/container.h"

namespace soil::world {
class World {
 public:
  struct Definition {
    float Friction{0.05F};
  };

  explicit World(const Definition& definition, volume::Container* container);
  ~World() = default;
  void Update();

  void Insert(entity::CollisionObject* object);
  void Remove(entity::CollisionObject* object);
  void Activate(entity::CollisionObject* object);

  void QueryObjectsAt(
      const glm::vec3& point,
      std::vector<const entity::CollisionObject*>& objects) const;

  void QueryObjectsInRange(
      const glm::vec3& point, float radius,
      std::vector<const entity::CollisionObject*>& objects) const;

  void Clear();

  int GetMaxIdleTime() const;
  void SetMaxIdleTime(int maxIdleTime);

  const soil::world::volume::Container* Container();

 protected:
  float friction_;
  int maxIdleTime_;
  struct ObjectState {
    entity::CollisionObject* Object{nullptr};
    int IdleTime{0};
  };
  std::vector<ObjectState> activeObjects_;
  volume::Container* container_;
};
}  // namespace soil::world
#endif
