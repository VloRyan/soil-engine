#ifndef ENGINE_SOUND_BUFFER_H
#define ENGINE_SOUND_BUFFER_H
#include <string>

namespace sound {
class Buffer {
public:
  explicit Buffer(std::string name);

  virtual ~Buffer();

  [[nodiscard]] std::string getName() const;

  [[nodiscard]] uint getId() const;

  [[nodiscard]] uint getDataSize() const;

  void setData(const void *data, int dataSize, int format, int frequency);

private:
  uint id_;
  std::string name_;

  uint dataSize_;
};
} // namespace sound
#endif /* ENGINE_SOUND_BUFFER_H */
