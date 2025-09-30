#include "sound/file.h"

#include <AL/al.h>

#include <utility>

namespace soil::sound {
File::File(std::string name, const ALenum format, const ALsizei dataSize,
           const ALsizei frequency, const fpos_t dataOffset, ALubyte *Data)
    : Name(std::move(name)),
      Format(format),
      Frequency(frequency),
      DataSize(dataSize),
      DataOffset(dataOffset),
      Data(Data) {}

File::File(std::string name)
    : Name(std::move(name)),
      Format(0),
      Frequency(0),
      DataSize(0),
      DataOffset(),
      Data(nullptr) {}

File::~File() { delete Data; }
}  // namespace soil::sound
