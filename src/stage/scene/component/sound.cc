
#include "stage/scene/component/sound.h"

#include "stage/scene/node.h"

namespace stage::scene::component {
Sound::Sound(::sound::Source *source)
    : Component(Type::Sound), source_(source) {}

void Sound::Play() const {
  if (source_ == nullptr) {
    return;
  }
  source_->Play();
}

void Sound::Stop() const {
  if (source_ == nullptr) {
    return;
  }
  source_->Stop();
}

::sound::Source *Sound::GetSource() const { return source_; }

void Sound::UpdateMatrix(const glm::mat4 &matrix) {
  Component::UpdateMatrix(matrix);
  if (source_ == nullptr) {
    return;
  }
  source_->SetPosition(matrix[3]);
}
} // namespace stage::scene::component
