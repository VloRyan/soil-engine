#ifndef SOIL_STAGE_SCENE_COMPONENT_BOUNDING_VOLUME_H_
#define SOIL_STAGE_SCENE_COMPONENT_BOUNDING_VOLUME_H_

#include <bitset>
#include <glm/glm.hpp>
#include <vector>

#include "component.h"
#include "world/entity/collision_object.h"
#include "world/volume/volume.hpp"
namespace soil::stage::scene::world {
class WorldNode;
}
namespace soil::stage::scene::component {

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

  void SetWorld(soil::stage::scene::world::WorldNode* world);

  [[nodiscard]] soil::stage::scene::world::WorldNode* GetWorld() const;

  void SetContactResponse(
      const std::function<void(CollisionObjectComponent* object,
                               const CollisionObjectComponent* other)>&
          response);

 protected:
  soil::world::entity::CollisionObject* object_;
  soil::stage::scene::world::WorldNode* world_;
  std::function<void(CollisionObjectComponent* object,
                     const CollisionObjectComponent* other)>
      contactResponse_;
};
}  // namespace soil::stage::scene::component

#endif
