#ifndef SOIL_ENGINE_TRANSFORM_COMPONENT_H
#define SOIL_ENGINE_TRANSFORM_COMPONENT_H
#include "component.h"
#include "world/entity/transform_3d.h"
namespace soil::stage::scene::component {
class TransformComponent : public Component {
 public:
  TransformComponent();
  ~TransformComponent() override = default;
  [[nodiscard]] virtual glm::vec3 GetPosition() const;
  virtual void SetPosition(const glm::vec3& pos);
  [[nodiscard]] virtual glm::vec3 GetLocalPosition() const;
  virtual void SetLocalPosition(const glm::vec3& pos);

  [[nodiscard]] virtual glm::vec3 GetDirection() const;
  // virtual void SetDirection(const glm::vec3& direction);

  [[nodiscard]] virtual glm::vec3 GetUp() const;
  // virtual void SetUp(const glm::vec3& up);

  [[nodiscard]] virtual glm::vec3 GetRight() const;
  // virtual void SetRight(const glm::vec3& right);
  // virtual void SetMatrix(const glm::mat4& transform);
  [[nodiscard]] virtual const glm::mat4& GetMatrix() const;
  [[nodiscard]] virtual const glm::mat4& GetLocalMatrix() const;
  virtual void SetLocalMatrix(const glm::mat4& transform);

  void UpdateTransform(const glm::mat4& parentTransform);

 private:
  soil::world::entity::Transform3d worldTransform_;
  soil::world::entity::Transform3d localTransform_;
};
}  // namespace soil::stage::scene::component
#endif
