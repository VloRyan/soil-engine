#ifndef SOIL_SOUND_LISTENER_H
#define SOIL_SOUND_LISTENER_H
#include "glm/glm.hpp"

namespace soil::sound {
class Listener {
 public:
  Listener() = default;

  virtual ~Listener() = default;

  virtual void SetOrientation(const glm::mat2x3& orientation) = 0;

  [[nodiscard]] virtual glm::mat2x3 getOrientation() const = 0;

  virtual void setVelocity(glm::vec3 velocity) = 0;

  [[nodiscard]] virtual glm::vec3 getVelocity() const = 0;

  virtual void SetPosition(glm::vec3 position) = 0;

  [[nodiscard]] virtual glm::vec3 getPosition() const = 0;

  virtual void setGain(float gain) = 0;

  [[nodiscard]] virtual float getGain() const = 0;
};
}  // namespace soil::sound

#endif
