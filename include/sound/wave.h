#ifndef SOIL_SOUND_WAVE_H
#define SOIL_SOUND_WAVE_H
#include <cstdint>

#include "file.h"

namespace soil::sound {
/**
 * Struct that holds the RIFF data of the Wave file.
 * The RIFF data is the metadata information that holds,
 * the ID, size and format of the wave file
 */
struct RIFF_Header {
  char chunkID[4];
  std::uint32_t chunkSize;  // size not including chunkSize or chunkID
  char format[4];
};

struct WAVE_Format {
  char subChunkID[4];
  std::uint32_t subChunkSize;
  std::uint16_t wFormatTag;
  std::uint16_t wChannels;
  std::uint32_t dwSamplesPerSec;
  std::uint32_t dwAvgBytesPerSec;
  std::uint16_t wBlockAlign;
  std::uint16_t wBitsPerSample;
};

/**
 * Struct to hold the data of the wave file
 */
struct WAVE_Data {
  char subChunkID[4];           // should contain the word data
  std::uint32_t subChunk2Size;  // Stores the size of the data block
};
class WaveFile : public File {
 public:
  struct Cursor : File::Cursor {
    explicit Cursor(const WaveFile* file);
    long Read(char* buffer, long bufferSize) override;
    void Rewind() override;
    void SetLoop(bool loop) override;
    long Offset{0};
    bool Loop{false};
    const WaveFile* File;
  };

  ~WaveFile() override = default;
  static WaveFile* Load(const std::string& file);
  Cursor* NewCursor() override;

 private:
  WaveFile(const std::string& name, const InfoType& info, long dataOffset);
  static void loadHeader(FILE* wavFile, InfoType& info);
  long dataOffset_;
};
}  // namespace soil::sound
#endif
