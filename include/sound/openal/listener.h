#ifndef SOIL_SOUND_OPENAL_LISTENER_H
#define SOIL_SOUND_OPENAL_LISTENER_H
#include "sound/listener.h"

namespace soil::sound::openal {
class Listener final : public sound::Listener {
 public:
  Listener();

  ~Listener() override = default;

  void SetOrientation(const glm::mat2x3& orientation) override;

  [[nodiscard]] glm::mat2x3 getOrientation() const override;

  void setVelocity(glm::vec3 velocity) override;

  [[nodiscard]] glm::vec3 getVelocity() const override;

  void SetPosition(glm::vec3 position) override;

  [[nodiscard]] glm::vec3 getPosition() const override;

  void setGain(float gain) override;

  [[nodiscard]] float getGain() const override;

 private:
  float gain_;
  glm::vec3 position_;
  glm::vec3 velocity_;
  glm::mat2x3 orientation_;
};
}  // namespace soil::sound::openal

#endif
