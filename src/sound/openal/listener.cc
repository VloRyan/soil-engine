
#include "sound/openal/listener.h"

#include "AL/al.h"

namespace soil::sound::openal {
Listener::Listener()
    : gain_(1.0F),
      position_(0),
      velocity_(0),
      orientation_(0.0F, 0.0F, -1.0F, 0.0F, 1.0F, 0.0F) {
  alListenerf(AL_GAIN, gain_);
  alListenerfv(AL_POSITION, &position_[0]);
  alListenerfv(AL_VELOCITY, &velocity_[0]);
  alListenerfv(AL_ORIENTATION, &orientation_[0][0]);
}

void Listener::SetOrientation(const glm::mat2x3 &orientation) {
  alListenerfv(AL_ORIENTATION, &orientation[0][0]);
  orientation_ = orientation;
}

glm::mat2x3 Listener::getOrientation() const { return orientation_; }

void Listener::setVelocity(const glm::vec3 velocity) {
  alListenerfv(AL_VELOCITY, &velocity_[0]);
  velocity_ = velocity;
}

glm::vec3 Listener::getVelocity() const { return velocity_; }

void Listener::SetPosition(const glm::vec3 position) {
  alListenerfv(AL_POSITION, &position[0]);
  position_ = position;
}

glm::vec3 Listener::getPosition() const { return position_; }

void Listener::setGain(const float gain) {
  alListenerf(AL_GAIN, gain);
  gain_ = gain;
}

float Listener::getGain() const { return gain_; }
}  // namespace soil::sound::openal
