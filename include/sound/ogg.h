#ifndef SOIL_ENGINE_OGG_H
#define SOIL_ENGINE_OGG_H
#include <cstddef>
#include <cstdint>
#include <string>

#include "file.h"
#include "vorbis/vorbisfile.h"

namespace soil::sound {

class OggFile : public File {
 public:
  struct Cursor : File::Cursor {
    OggFile* File;
    long Consumed{0};
    OggVorbis_File OggVorbisFile;
    int OggCurrentSection{0};
    bool Loop{false};
    long Read(char* buffer, long bufferSize) override;
    void Rewind() override;
    void SetLoop(bool loop) override;
    [[nodiscard]] long FileSize() const;
    explicit Cursor(OggFile* file);
  };
  ~OggFile() override = default;
  Cursor* NewCursor() override;
  static OggFile* Load(const std::string& fileName);

 protected:
  OggFile(const std::string& filename, const InfoType& info);

  static bool loadInfo(const std::string& filename, int& dataSize,
                       InfoType& info);
  int Size{0};
};

static std::size_t read_ogg_callback(void* destination, std::size_t size1,
                                     std::size_t size2, void* fileHandle);

static std::int32_t seek_ogg_callback(void* fileHandle, ogg_int64_t to,
                                      std::int32_t type);
static long int tell_ogg_callback(void* fileHandle);

}  // namespace soil::sound
#endif
