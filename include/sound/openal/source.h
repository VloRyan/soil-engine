#ifndef SOIL_SOUND_OPENAL_SOURCE_H
#define SOIL_SOUND_OPENAL_SOURCE_H
#include "sound/buffer.h"
#include "sound/file.h"
#include "sound/source.h"

namespace soil::sound::openal {
static inline auto MAX_BUFFERS = 4;
static inline auto BUFFER_SIZE = 65536;
class Source : public sound::Source {
 public:
  explicit Source(const File* file, bool loop = false);

  ~Source() override;

  void Update() override;

  [[nodiscard]] glm::vec3 GetPosition() const override;

  void SetPosition(glm::vec3 position) override;

  [[nodiscard]] float GetPitch() const override;

  void SetPitch(float) override;

  [[nodiscard]] float GetGain() const override;

  void SetGain(float) override;

  [[nodiscard]] bool IsPlaying() const override;

  void SetLooping(bool doLoop) override;

  [[nodiscard]] bool GetLooping() const override;

  void SetSourceRelative(bool relative) override;

  [[nodiscard]] bool GetSourceRelative() const override;

  [[nodiscard]] float GetMaxDistance() const override;

  void SetMaxDistance(float distance) override;

  [[nodiscard]] float GetRolloffFactor() const override;

  void SetRolloffFactor(float factor) override;

  [[nodiscard]] float GetReferenceDistance() const override;

  void SetReferenceDistance(float distance) override;

  void Play() override;

  void Pause() override;

  void Rewind() override;

  void Stop() override;

  PlayStateType GetPlayState() const override;

  const File* GetFile() const override;

 protected:
  void SetPlayState(PlayStateType playState);
  [[nodiscard]] uint GetId() const;

  uint id_;
  std::vector<Buffer*> buffers_;
  PlayStateType playState_;
  const File* file_;
  File::Cursor* cursor_;
  bool loop_;
};
}  // namespace soil::sound::openal
#endif
