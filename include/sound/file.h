#ifndef SOIL_SOUND_FILE_H
#define SOIL_SOUND_FILE_H
#include <string>

namespace soil::sound {

class File {
 public:
  class Cursor {
   public:
    virtual ~Cursor() = default;
    virtual long Read(char* buffer, long bufferSize) = 0;
    virtual void Rewind() = 0;
    virtual void SetLoop(bool loop) = 0;

   protected:
    Cursor() = default;
  };
  enum class BufferFormatType {
    Mono8 = 0,
    Mono16,
    Stereo8,
    Stereo16,
  };
  struct InfoType final {
    BufferFormatType Format{0};
    int Samplerate{0};
    long DataSize{0L};
  };
  virtual ~File() = default;
  [[nodiscard]] const std::string& Name() const;
  [[nodiscard]] const InfoType& Info() const;
  virtual Cursor* NewCursor() = 0;

 protected:
  File() = default;
  File(std::string name, const InfoType& info);
  InfoType info_;
  std::string name_;
};
}  // namespace soil::sound
#endif
