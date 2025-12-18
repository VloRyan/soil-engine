
#include "video/texture/animation.h"

#include "engine.h"

namespace soil::video::texture {
Animation::Frame Animation::EMPTY_FRAME = {};
Animation::Animation(const std::vector<Frame>* frames, const int ticksPerSecond)
    : tickDuration_(1.F / static_cast<float>(ticksPerSecond) * 1000.F),
      currentDuration_(0.F),
      currentFrameIndex_(0),
      frames_(frames) {}

const Animation::Frame& Animation::Update() {
  if (frames_ == nullptr) {
    return EMPTY_FRAME;
  }
  currentDuration_ += tickDuration_;
  const Animation::Frame& currentFrame = frames_->at(currentFrameIndex_);
  if (currentDuration_ > static_cast<float>(currentFrame.Duration)) {
    currentDuration_ = 0.F;
    currentFrameIndex_++;
    if (currentFrameIndex_ == frames_->size()) {
      currentFrameIndex_ = 0;
      return frames_->at(currentFrameIndex_);
    }
  }
  return currentFrame;
}

const Animation::Frame& Animation::CurrentFrame() const {
  if (frames_ == nullptr) {
    return EMPTY_FRAME;
  }
  return frames_->at(currentFrameIndex_);
}

void Animation::SetFrameIndex(const int index) {
  if (frames_ == nullptr) {
    currentFrameIndex_ = 0;
    return;
  }
  if (index < 0 || index >= static_cast<int>(frames_->size())) {
    return;
  }
  currentFrameIndex_ = index;
}

const std::vector<Animation::Frame>* Animation::Frames() const {
  return frames_;
}

float Animation::TickDuration() const { return tickDuration_; }

void Animation::SetFrames(const std::vector<Frame>* frames) {
  if (frames_ == frames) {
    return;
  }
  frames_ = frames;
  currentFrameIndex_ = 0;
}

}  // namespace soil::video::texture
