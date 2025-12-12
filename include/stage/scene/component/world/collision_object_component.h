#ifndef SOIL_STAGE_SCENE_COMPONENT_WORLD_COLLISION_OBJECT_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_WORLD_COLLISION_OBJECT_COMPONENT_H

#include <bitset>
#include <glm/glm.hpp>
#include <vector>

#include "stage/scene/component/component.h"
#include "world/entity/collision_object.h"
#include "world/volume/volume.hpp"

namespace soil::stage::scene::component::world {
class WorldComponent;

class CollisionObjectComponent final : public Component {
 public:
  explicit CollisionObjectComponent(
      soil::world::volume::Volume* volume,
      soil::world::entity::CollisionObject::ContactType contactType,
      const std::vector<soil::world::entity::CollisionObject::ContactType>&
          contactTypes = {});

  ~CollisionObjectComponent() override = default;

  void Update() override;

  soil::world::entity::CollisionObject* Object() const;

  virtual glm::vec3 GetPosition() const;
  virtual void SetPosition(const glm::vec3& pos);

  virtual glm::vec3 GetVelocity() const;

  virtual void SetVelocity(const glm::vec3& velocity);

  void SetParent(Node* parent) override;

  void SetWorld(WorldComponent* world);

  [[nodiscard]] WorldComponent* GetWorld() const;

  void SetContactResponse(
      const std::function<void(CollisionObjectComponent* object,
                               const CollisionObjectComponent* other)>&
          response);

 protected:
  soil::world::entity::CollisionObject* object_;
  WorldComponent* world_;
  std::function<void(CollisionObjectComponent* object,
                     const CollisionObjectComponent* other)>
      contactResponse_;
};
}  // namespace soil::stage::scene::component::world

#endif
