#ifndef SOIL_STAGE_SCENE_VIEWER_GUI_H_
#define SOIL_STAGE_SCENE_VIEWER_GUI_H_

#include "node.h"

namespace soil::stage::scene::viewer {
enum class OrthoType { OrthoHeight, Full };

class Ortho final : public Node {
 public:
  explicit Ortho(glm::ivec2 resolution);

  ~Ortho() override = default;

  void Look(glm::vec3 pos, glm::vec3 center,
            glm::vec3 up = glm::vec3(0.0F, 1.0F, 0.0F));

  [[nodiscard]] OrthoType GetOrthoType() const;

  void SetOrthoType(OrthoType orthoType);

  [[nodiscard]] glm::mat4 GetViewMatrix() const override;

  [[nodiscard]] glm::mat4 GetProjectionMatrix() const override;

  void Update() override;

 protected:
  void UpdateProjection(const glm::ivec2& size) override;

 private:
  void UpdateDirty() override;

  glm::mat4 projection_;
  glm::mat4 view_;

  float orthoSize_;
  OrthoType orthoType_;
};
}  // namespace soil::stage::scene::viewer

#endif
