#ifndef SOIL_SOUND_BUFFER_H
#define SOIL_SOUND_BUFFER_H
#include <string>

namespace soil::sound {
class Buffer {
 public:
  explicit Buffer() = default;

  virtual ~Buffer() = default;

  [[nodiscard]] virtual uint Id() const = 0;

  [[nodiscard]] virtual int DataSize() const = 0;

  virtual void SetData(const void* data, int dataSize, int format,
                       int samplerate) = 0;
};
}  // namespace soil::sound
#endif
