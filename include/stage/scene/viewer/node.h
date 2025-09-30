#ifndef SOIL_STAGE_SCENE_VIEWER_NODE_H
#define SOIL_STAGE_SCENE_VIEWER_NODE_H

#include "glm/glm.hpp"
#include "stage/scene/node.h"
#include "world/volume/frustum.h"

namespace soil::stage::scene::viewer {
class Node : public scene::Node {
 public:
  ~Node() override;

  [[nodiscard]] virtual glm::mat4 GetViewMatrix() const = 0;

  [[nodiscard]] virtual glm::mat4 GetProjectionMatrix() const = 0;

  [[nodiscard]] virtual float GetNearZ() const;

  virtual void SetNearZ(float nearZ);

  [[nodiscard]] virtual float GetFarZ() const;

  virtual void SetFarZ(float farZ);

  virtual world::volume::Frustum* GetFrustum();

  [[nodiscard]] glm::vec3 GetDirection() const override;

  [[nodiscard]] glm::vec3 GetRight() const override;

  [[nodiscard]] glm::vec3 GetUp() const override;

  void Handle(const WindowEvent& event) override;

 protected:
  Node();

  virtual void UpdateProjection(const glm::ivec2& size) = 0;

  glm::vec3 direction_;
  glm::vec3 right_;
  glm::vec3 up_;

  float nearZ_;
  float farZ_;

  glm::ivec2 windowSize_;

  world::volume::Frustum* frustum_;
};
}  // namespace soil::stage::scene::viewer
#endif
