#ifndef SOIL_WORLD_ENTITY_OBJECT_3D_H_
#define SOIL_WORLD_ENTITY_OBJECT_3D_H_

#include <glm/glm.hpp>

namespace soil::world::entity {
class Object3d {
 public:
  Object3d(const Object3d& other) = delete;

  Object3d(Object3d&& other) noexcept = delete;

  Object3d();

  virtual ~Object3d();

  [[nodiscard]] virtual glm::vec3 GetPosition() const;

  virtual void SetPosition(const glm::vec3& pos);

  [[nodiscard]] virtual glm::vec3 GetDirection() const;

  virtual void SetDirection(const glm::vec3& direction);

  [[nodiscard]] virtual glm::vec3 GetUp() const;

  virtual void SetUp(const glm::vec3& up);

  [[nodiscard]] virtual glm::vec3 GetRight() const;

  virtual void SetRight(const glm::vec3& right);
  virtual void SetTransform(const glm::mat4& transform);
  [[nodiscard]] virtual const glm::mat4& GetTransform() const;

 private:
  glm::mat4 transform_;
};
}  // namespace soil::world::entity

#endif
