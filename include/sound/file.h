#ifndef ENGINE_SOUND_FILE_H
#define ENGINE_SOUND_FILE_H
#include <string>

namespace sound {
struct File {
  explicit File(std::string name, int format, int dataSize, int frequency,
                fpos_t dataOffset, unsigned char *Data);

  explicit File(std::string name);

  virtual ~File();

  std::string Name;
  int Format;
  int Frequency;
  int DataSize;
  fpos_t DataOffset;
  unsigned char *Data;
};
} // namespace sound
#endif // ENGINE_SOUND_FILE_H
