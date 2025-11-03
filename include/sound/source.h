#ifndef SOIL_SOUND_SOURCE_H
#define SOIL_SOUND_SOURCE_H
#include "event/observable.hpp"
#include "event/source_event.hpp"
#include "file.h"
#include "glm/vec3.hpp"

namespace soil::sound {
class Source : public soil::event::Observable<event::Event> {
 public:
  enum class PlayStateType : std::uint8_t {
    Playing = 0,
    Stopped = 1,
  };

  ~Source() override = default;

  virtual void Update() = 0;

  [[nodiscard]] virtual glm::vec3 GetPosition() const = 0;

  virtual void SetPosition(glm::vec3 position) = 0;

  [[nodiscard]] virtual float GetPitch() const = 0;

  virtual void SetPitch(float) = 0;

  [[nodiscard]] virtual float GetGain() const = 0;

  virtual void SetGain(float) = 0;

  [[nodiscard]] virtual bool IsPlaying() const = 0;

  virtual void SetLooping(bool doLoop) = 0;

  [[nodiscard]] virtual bool GetLooping() const = 0;

  virtual void SetSourceRelative(bool relative) = 0;

  [[nodiscard]] virtual bool GetSourceRelative() const = 0;

  [[nodiscard]] virtual float GetMaxDistance() const = 0;

  virtual void SetMaxDistance(float distance) = 0;

  [[nodiscard]] virtual float GetRolloffFactor() const = 0;

  virtual void SetRolloffFactor(float factor) = 0;

  [[nodiscard]] virtual float GetReferenceDistance() const = 0;

  virtual void SetReferenceDistance(float distance) = 0;

  virtual void Play() = 0;

  virtual void Pause() = 0;

  virtual void Rewind() = 0;

  virtual void Stop() = 0;

  virtual PlayStateType GetPlayState() const = 0;
  virtual const File* GetFile() const = 0;

 protected:
  explicit Source() = default;
};
}  // namespace soil::sound
#endif
