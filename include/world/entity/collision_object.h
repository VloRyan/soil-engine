#ifndef SOIL_WORLD_ENTITY_COLLISION_OBJECT_H
#define SOIL_WORLD_ENTITY_COLLISION_OBJECT_H
#include "bitset"
#include "functional"
#include "world/entity/object_3d.h"
#include "world/volume/volume.hpp"
namespace soil::world::entity {
class CollisionObject : public Object3d {
 public:
  enum class ContactType {
    /** Ghost can only be seen but does not collide*/
    Ghost = 0,
    /** StaticObject's are non-moving objects*/
    StaticObject = 1,
    /** Object's can move*/
    Object = 2,
  };
  enum class State { Inactive = 0, Idle, Active };

  inline static std::vector<ContactType> ALL_CONTACT_TYPES = {
      ContactType::Ghost, ContactType::StaticObject, ContactType::Object};

  explicit CollisionObject(volume::Volume* volume, ContactType contactType,
                           const std::vector<ContactType>& contactTypes = {});
  ~CollisionObject() override = default;

  [[nodiscard]] virtual glm::vec3 GetVelocity() const;
  virtual void SetVelocity(const glm::vec3& velocity);
  virtual void HandleCollision(const entity::CollisionObject* other);
  [[nodiscard]] virtual volume::Volume* GetVolume() const;

  [[nodiscard]] bool CanCollideWith(ContactType type) const;

  void SetCollideWith(ContactType type, bool value);

  [[nodiscard]] ContactType GetContactType() const;

  [[nodiscard]] virtual bool IsInside(const glm::vec3& min,
                                      const glm::vec3& max) const;

  [[nodiscard]] virtual bool IsInsideXZ(const glm::vec3& min,
                                        const glm::vec3& max) const;

  [[nodiscard]] virtual bool IsInside(const glm::vec2& min,
                                      const glm::vec2& max) const;

  [[nodiscard]] virtual bool Contains(const glm::vec3& point) const;

  [[nodiscard]] virtual bool ContainsXZ(const glm::vec3& point) const;

  [[nodiscard]] virtual volume::IntersectionResult IntersectsRay(
      const glm::vec3& start, const glm::vec3& dir) const;

  [[nodiscard]] virtual volume::IntersectionResult IntersectsRayXZ(
      const glm::vec3& start, const glm::vec3& dir) const;

  [[nodiscard]] virtual bool IntersectsCircle(const glm::vec2& circleCenter,
                                              float radius) const;

  [[nodiscard]] virtual std::vector<volume::Line> GenerateLines() const;

  void SetOnWorldUpdate(const std::function<void()>& onWorldUpdate);

  void SetPosition(const glm::vec3& position) override;

  [[nodiscard]] State GetState() const;
  void SetState(State state);

  void Update(const glm::vec3& position, const glm::vec3& velocity);

  void SetContactResponse(
      const std::function<void(entity::CollisionObject*,
                               const entity::CollisionObject*)>&
          contactResponse);

 protected:
  State state_;
  glm::vec3 velocity_;
  volume::Volume* volume_;
  std::bitset<3> collideWithTypes_;
  ContactType contactType_;
  std::function<void()> onWorldUpdate_;
  std::function<void(entity::CollisionObject* object,
                     const entity::CollisionObject* other)>
      contactResponse_;
};
}  // namespace soil::world::entity
#endif
