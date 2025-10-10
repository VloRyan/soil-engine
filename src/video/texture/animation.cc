
#include "video/texture/animation.h"

#include "engine.h"

namespace soil::video::texture {
Animation::Animation(const file::Sequence* sequence)
    : duration_(0.F), currentFrame_(0), sequence_(sequence) {}

int Animation::Update() {
  if (sequence_ == nullptr) {
    return -1;
  }
  const auto tickDuration =
      1.F / static_cast<float>(Engine::Config().TicksPerSecond) * 1000.F;
  duration_ += tickDuration;
  if (duration_ >= static_cast<float>(sequence_->FrameDuration)) {
    duration_ = 0.F;
    currentFrame_++;
    if (currentFrame_ > sequence_->To) {
      currentFrame_ = sequence_->From;
    }
  }
  return currentFrame_;
}

int Animation::GetCurrentFrame() const { return currentFrame_; }

void Animation::SetCurrentFrame(const int currentFrame) {
  currentFrame_ = currentFrame;
}

const file::Sequence* Animation::GetSequence() const { return sequence_; }

void Animation::SetSequence(const file::Sequence* const sequence) {
  if (sequence_ == sequence) {
    return;
  }
  sequence_ = sequence;
  currentFrame_ = sequence_->From;
  duration_ = 0.F;
}
}  // namespace soil::video::texture
