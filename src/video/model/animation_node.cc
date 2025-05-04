#include "video/model/animation_node.h"
#include "exception.h"

#include <algorithm>
#include <glm/ext/quaternion_common.hpp>
#include <utility>

namespace video::model {
AnimationNode::AnimationNode(std::string name) : nodeName_(std::move(name)) {}

AnimationNode::~AnimationNode() = default;

std::string AnimationNode::getNodeName() const { return nodeName_; }

uint AnimationNode::getNumPositionKeys() const {
  return static_cast<uint>(positionKeys_.size());
}

uint AnimationNode::getNumRotationKeys() const {
  return static_cast<uint>(rotationKeys_.size());
}

uint AnimationNode::getNumScalingKeys() const {
  return static_cast<uint>(scalingKeys_.size());
}

std::vector<AnimationNode::VectorKey> AnimationNode::getPositionKeys() const {
  return positionKeys_;
}

std::vector<AnimationNode::QuatKey> AnimationNode::getRotationKeys() const {
  return rotationKeys_;
}

std::vector<AnimationNode::VectorKey> AnimationNode::getScalingKeys() const {
  return scalingKeys_;
}

uint AnimationNode::FindPosition(const float AnimationTime) const {
  for (uint i = 0; i < positionKeys_.size() - 1; ++i) {
    if (AnimationTime < static_cast<float>(positionKeys_[i + 1].Time)) {
      return i;
    }
  }
  return static_cast<uint>(positionKeys_.size()) - 2U;
  // throw_assert(0, "Can't find position");
  // return 0;
}

uint AnimationNode::FindRotation(const float AnimationTime) const {
  if (getNumRotationKeys() > 0) {
    throw engine::Exception("FindRotation");
  }

  for (uint i = 0; i < rotationKeys_.size() - 1; ++i) {
    if (AnimationTime < static_cast<float>(rotationKeys_[i + 1].Time)) {
      return i;
    }
  }
  return static_cast<uint>(rotationKeys_.size()) - 2U;
  // throw_assert(0, "Can't find rotation");
  // return 0;
}

uint AnimationNode::FindScaling(const float AnimationTime) const {
  if (getNumScalingKeys() > 0) {
    throw engine::Exception("FindScaling");
  }

  for (uint i = 0; i < scalingKeys_.size() - 1; ++i) {
    if (AnimationTime < static_cast<float>(scalingKeys_[i + 1].Time)) {
      return i;
    }
  }
  return static_cast<uint>(positionKeys_.size()) - 2U;
  //    throw_assert(0, "Can't find scaling");
  //  return 0;
}

void AnimationNode::CalcInterpolatedPosition(glm::vec3 &Out,
                                             const float AnimationTime) const {
  if (positionKeys_.size() == 1) {
    Out = positionKeys_[0].Value;
    return;
  }

  const uint PositionIndex = FindPosition(AnimationTime);
  const uint NextPositionIndex = (PositionIndex + 1);
  if (NextPositionIndex < getNumPositionKeys()) {
    throw engine::Exception("CalcInterpolatedPosition");
  }
  const auto DeltaTime =
      static_cast<float>(positionKeys_[NextPositionIndex].Time -
                         positionKeys_[PositionIndex].Time);
  float Factor =
      (AnimationTime - static_cast<float>(positionKeys_[PositionIndex].Time)) /
      DeltaTime;
  Factor = std::clamp(Factor, 0.0F, 1.0F);
  const glm::vec3 &Start = positionKeys_[PositionIndex].Value;
  const glm::vec3 &End = positionKeys_[NextPositionIndex].Value;
  const glm::vec3 Delta = End - Start;
  Out = Start + Factor * Delta;
}

void AnimationNode::CalcInterpolatedRotation(glm::quat &Out,
                                             const float AnimationTime) const {
  // we need at least two values to interpolate...
  if (rotationKeys_.size() == 1) {
    Out = rotationKeys_[0].Value;
    return;
  }

  const uint RotationIndex = FindRotation(AnimationTime);
  const uint NextRotationIndex = (RotationIndex + 1);
  if (NextRotationIndex < getNumRotationKeys()) {
    throw engine::Exception("CalcInterpolatedRotation");
  }
  const auto DeltaTime =
      static_cast<float>(rotationKeys_[NextRotationIndex].Time -
                         rotationKeys_[RotationIndex].Time);
  float Factor =
      (AnimationTime - static_cast<float>(rotationKeys_[RotationIndex].Time)) /
      DeltaTime;
  Factor = std::clamp(Factor, 0.0F, 1.0F);
  const glm::quat &StartRotationQ = rotationKeys_[RotationIndex].Value;
  const glm::quat &EndRotationQ = rotationKeys_[NextRotationIndex].Value;
  Out = glm::slerp(StartRotationQ, EndRotationQ, Factor);
  Out = glm::normalize(Out);
}

void AnimationNode::CalcInterpolatedScaling(glm::vec3 &Out,
                                            float AnimationTime) const {
  if (scalingKeys_.size() == 1) {
    Out = scalingKeys_[0].Value;
    return;
  }

  const uint ScalingIndex = FindScaling(AnimationTime);
  const uint NextScalingIndex = (ScalingIndex + 1);
  if (NextScalingIndex < getNumScalingKeys()) {
    throw engine::Exception("CalcInterpolatedScaling");
  }
  const auto DeltaTime = static_cast<float>(
      scalingKeys_[NextScalingIndex].Time - scalingKeys_[ScalingIndex].Time);
  float Factor =
      (AnimationTime - static_cast<float>(scalingKeys_[ScalingIndex].Time)) /
      DeltaTime;
  Factor = std::clamp(Factor, 0.0F, 1.0F);
  const glm::vec3 &Start = scalingKeys_[ScalingIndex].Value;
  const glm::vec3 &End = scalingKeys_[NextScalingIndex].Value;
  const glm::vec3 Delta = End - Start;
  Out = Start + Factor * Delta;
}
} // namespace video::model
