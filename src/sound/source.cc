#include "sound/source.h"

#include <al.h>
#include <cmath>
#include <glm/vec3.hpp>

namespace sound {
Source::Source(Buffer *buffer) : id_(0), buffer_(nullptr) {
  alGenSources(1, &id_);
  SetBuffer(buffer);
}

Source::~Source() {
  if (this->id_ > 0) {
    alDeleteSources(1, &this->id_);
  }
}

glm::vec3 Source::GetPosition() const {
  glm::vec3 pos;
  alGetSourcefv(this->id_, AL_POSITION, &pos[0]);
  return pos;
}

void Source::SetPosition(glm::vec3 position) {
  alSourcefv(this->id_, AL_POSITION, &position[0]);
  // //AudioUtils::outputError(LOGGER, "Error after setPosition");
}

float Source::GetPitch() const {
  float pitch = NAN;
  alGetSourcef(this->id_, AL_PITCH, &pitch);
  return pitch;
}

void Source::SetPitch(const float pitch) {
  alSourcef(this->id_, AL_PITCH, pitch);

  ////AudioUtils::outputError(LOGGER, "Error after setPitch");
}

float Source::GetGain() const {
  float gain = NAN;
  alGetSourcef(this->id_, AL_GAIN, &gain);
  return gain;
}

void Source::SetGain(const float gain) {
  alSourcef(this->id_, AL_GAIN, gain);
  // AudioUtils::outputError(LOGGER, "Error after setGain");
}

uint Source::GetId() const { return id_; }

bool Source::IsPlaying() const {
  ALint state = 0;
  alGetSourcei(this->id_, AL_SOURCE_STATE, &state);
  return state == AL_PLAYING;
}

void Source::SetLooping(const bool doLoop) {
  if (doLoop) {
    alSourcei(this->id_, AL_LOOPING, AL_TRUE);
  } else {
    alSourcei(this->id_, AL_LOOPING, AL_FALSE);
  }
  // AudioUtils::outputError(LOGGER, "Error after setLooping");
}

bool Source::GetLooping() const {
  ALint looping = 0;
  alGetSourcei(this->id_, AL_LOOPING, &looping);
  return looping == AL_TRUE;
}

void Source::SetSourceRelative(const bool relative) {
  alSourcei(this->id_, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
  // AudioUtils::outputError(LOGGER, "Error after setSourceRelative");
}

bool Source::GetSourceRelative() const {
  ALint relative = 0;
  alGetSourcei(this->id_, AL_SOURCE_RELATIVE, &relative);
  return relative == AL_TRUE;
}

void Source::Play() {
  alSourcePlay(this->id_);
  // AudioUtils::outputError(LOGGER, "Error after play");
}

void Source::Pause() {
  alSourcePause(this->id_);
  // AudioUtils::outputError(LOGGER, "Error after pause");
}

void Source::Rewind() {
  alSourceRewind(this->id_);
  // AudioUtils::outputError(LOGGER, "Error after rewind");
}

void Source::Stop() {
  alSourceStop(this->id_);
  // AudioUtils::outputError(LOGGER, "Error after stop");
}

void Source::SetBuffer(Buffer *Buffer) {
  if (buffer_ == Buffer) {
    return;
  }
  buffer_ = Buffer;
  if (buffer_ != nullptr) {
    alSourcei(id_, AL_BUFFER, static_cast<ALint>(buffer_->getId()));
  } else {
    alSourcei(id_, AL_BUFFER, 0);
  }
  // AudioUtils::outputError(LOGGER, "Error after setBuffer");
}

Buffer *Source::GetBuffer() const { return buffer_; }

float Source::GetMaxDistance() const {
  float distance = NAN;
  alGetSourcef(id_, AL_MAX_DISTANCE, &distance);
  return distance;
}

void Source::SetMaxDistance(float distance) {
  alSourcef(id_, AL_MAX_DISTANCE, distance);
  // AudioUtils::outputError(LOGGER, "Error after setMaxDistance");
}

float Source::GetReferenceDistance() const {
  float distance = NAN;
  alGetSourcef(id_, AL_REFERENCE_DISTANCE, &distance);
  return distance;
}

void Source::SetReferenceDistance(const float distance) {
  alSourcef(id_, AL_REFERENCE_DISTANCE, distance);
  // AudioUtils::outputError(LOGGER, "Error after setReferenceDistance");
}

float Source::GetRolloffFactor() const {
  float factor = NAN;
  alGetSourcef(id_, AL_ROLLOFF_FACTOR, &factor);
  return factor;
}

void Source::SetRolloffFactor(const float factor) {
  alSourcef(id_, AL_ROLLOFF_FACTOR, factor);
  // AudioUtils::outputError(LOGGER, "Error after setRolloffFactor");
}
} // namespace sound
