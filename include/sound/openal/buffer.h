#ifndef SOIL_SOUND_OPENAL_BUFFER_H
#define SOIL_SOUND_OPENAL_BUFFER_H
#include <al.h>

#include <string>

#include "sound/buffer.h"

namespace soil::sound::openal {
class Buffer final : public sound::Buffer {
 public:
  explicit Buffer();

  ~Buffer() override;

  [[nodiscard]] ALuint Id() const override;

  [[nodiscard]] ALsizei DataSize() const override;

  void SetData(const ALvoid *data, ALsizei dataSize, ALenum format,
               ALsizei samplerate) override;

 private:
  ALuint id_;
  ALsizei dataSize_;
};
}  // namespace soil::sound::openal
#endif
