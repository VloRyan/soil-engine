
#ifndef SOIL_VIDEO_TEXTURE_ANIMATION_H
#define SOIL_VIDEO_TEXTURE_ANIMATION_H
#include "file/sequence.hpp"

namespace soil::video::texture {
class Animation {
 public:
  explicit Animation(const file::Sequence* sequence);

  ~Animation() = default;

  int Update();

  [[nodiscard]] int GetCurrentFrame() const;

  void SetCurrentFrame(int currentFrame);

  [[nodiscard]] const file::Sequence* GetSequence() const;

  void SetSequence(const file::Sequence* sequence);

 private:
  float duration_;
  int currentFrame_;
  const file::Sequence* sequence_;
};
}  // namespace soil::video::texture

#endif
