
#ifndef ENGINE_SOUND_MANAGER_H
#define ENGINE_SOUND_MANAGER_H

#include <string>

#include "../base.h"
#include "sound/buffer.h"
#include "sound/listener.h"
#include "source.h"
class ALCdevice;
class ALCcontext;

namespace sound {
class Manager {
public:
  enum class DistanceModel : std::uint8_t {
    INVERSE_DISTANCE,
    INVERSE_DISTANCE_CLAMPED,
    LINEAR_DISTANCE,
    LINEAR_DISTANCE_CLAMPED,
    EXPONENT_DISTANCE,
    EXPONENT_DISTANCE_CLAMPED,
    NONE
  };

  Manager();

  ~Manager();

  void Init();

  Source *GetSource(const std::string &fileName);

  Buffer *GetBuffer(const std::string &fileName);

private:
  static Buffer *loadAudioFile(const std::string &filename);

  static void logErrors();

  ALCdevice *device_;
  ALCcontext *context_;
  Listener *listener_;

public:
  [[nodiscard]] Listener *GetListener() const;

private:
  HashMap<std::string, Buffer *> bufferCache_;
};
} // namespace sound

#endif // ENGINE_SOUND_MANAGER_H
