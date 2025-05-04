#include "video/model/animation.h"

#include <utility>

namespace video::model {
Animation::Animation(std::string name)
    : name_(std::move(name)), duration_(0.0), ticksPerSecond_(0.0) {}

Animation::~Animation() {
  for (const AnimationNode *channel : channels_) {
    delete channel;
  }
}

std::vector<AnimationNode *> Animation::getChannels() const {
  return channels_;
}

double Animation::getDuration() const { return duration_; }

std::string Animation::getName() const { return name_; }

uint Animation::getNumChannels() const {
  return static_cast<uint>(channels_.size());
}

double Animation::getTicksPerSecond() const { return ticksPerSecond_; }

void Animation::setDuration(double Duration) { duration_ = Duration; }

void Animation::setTicksPerSecond(double TicksPerSecond) {
  ticksPerSecond_ = TicksPerSecond;
}

const AnimationNode *Animation::findNode(const std::string &name) const {
  for (const auto *const pNodeAnim : channels_) {
    if (pNodeAnim->getNodeName() == name) {
      return pNodeAnim;
    }
  }
  return nullptr;
}
} // namespace video::model
