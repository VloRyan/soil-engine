#ifndef SOIL_WORLD_TRANSFORM_3D_H
#define SOIL_WORLD_TRANSFORM_3D_H
#include <glm/glm.hpp>
namespace soil::world::entity {
class Transform3d {
 public:
  Transform3d(const Transform3d& other) = delete;

  Transform3d(Transform3d&& other) noexcept = delete;

  Transform3d();

  virtual ~Transform3d();

  [[nodiscard]] virtual glm::vec3 GetPosition() const;

  virtual void SetPosition(const glm::vec3& pos);

  [[nodiscard]] virtual glm::vec3 GetDirection() const;

  virtual void SetDirection(const glm::vec3& direction);

  [[nodiscard]] virtual glm::vec3 GetUp() const;

  virtual void SetUp(const glm::vec3& up);

  [[nodiscard]] virtual glm::vec3 GetRight() const;
  virtual void SetRight(const glm::vec3& right);
  virtual void SetMatrix(const glm::mat4& transform);
  [[nodiscard]] virtual const glm::mat4& GetMatrix() const;

 private:
  glm::mat4 matrix_;
};
}  // namespace soil::world::entity
#endif
