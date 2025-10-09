#include "sound/openal/source.h"

#include <al.h>

#include <cmath>
#include <glm/vec3.hpp>

#include "sound/openal/buffer.h"

namespace soil::sound::openal {
Source::Source(const File* file, const bool loop)
    : id_(0),
      playState_(PlayStateType::Stopped),
      file_(file),
      cursor_(nullptr),
      loop_(loop) {
  if (file_ == nullptr) {
    throw std::runtime_error("[Source::Source] : file is nullptr");
  }

  alGenSources(1, &id_);

  const auto numBuffers = std::min(
      static_cast<int>(std::ceilf(static_cast<float>(file_->Info().DataSize) /
                                  static_cast<float>(BUFFER_SIZE))),
      MAX_BUFFERS);
  buffers_.resize(numBuffers);
  cursor_ = file->NewCursor();
  cursor_->SetLoop(loop_);
  auto* dataBuffer = new char[BUFFER_SIZE];
  if (numBuffers == 1) {
    const auto size = cursor_->Read(dataBuffer, BUFFER_SIZE);
    buffers_[0] = new openal::Buffer();
    buffers_[0]->SetData(
        dataBuffer, static_cast<int>(size),
        AL_FORMAT_MONO8 + static_cast<int>(file_->Info().Format),
        file_->Info().Samplerate);
    alSourcei(id_, AL_BUFFER, static_cast<ALint>(buffers_.front()->Id()));
  } else {
    uint bufferIds[numBuffers];
    for (auto i = 0; i < numBuffers; i++) {
      const auto size = cursor_->Read(dataBuffer, BUFFER_SIZE);
      buffers_[i] = new openal::Buffer();
      buffers_[i]->SetData(
          dataBuffer, static_cast<int>(size),
          AL_FORMAT_MONO8 + static_cast<int>(file_->Info().Format),
          file_->Info().Samplerate);
      bufferIds[i] = static_cast<ALint>(buffers_[i]->Id());
    }
    alSourceQueueBuffers(id_, numBuffers, &bufferIds[0]);
  }
  delete[] dataBuffer;
}

Source::~Source() {
  for (const auto* buffer : buffers_) {
    delete buffer;
  }
  Observable::fire(
      event::SourceEvent(event::SourceEvent::TriggerType::Removed, this));
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
}

float Source::GetPitch() const {
  float pitch = NAN;
  alGetSourcef(this->id_, AL_PITCH, &pitch);
  return pitch;
}

void Source::SetPitch(const float pitch) {
  alSourcef(this->id_, AL_PITCH, pitch);
}

float Source::GetGain() const {
  float gain = NAN;
  alGetSourcef(this->id_, AL_GAIN, &gain);
  return gain;
}

void Source::SetGain(const float gain) { alSourcef(this->id_, AL_GAIN, gain); }

uint Source::GetId() const { return id_; }

bool Source::IsPlaying() const {
  return GetPlayState() == PlayStateType::Playing;
}

void Source::Update() {
  ALint state = 0;
  alGetSourcei(this->id_, AL_SOURCE_STATE, &state);
  const auto newState =
      state == AL_PLAYING ? PlayStateType::Playing : PlayStateType::Stopped;
  SetPlayState(newState);

  if (newState == PlayStateType::Stopped) {
    return;
  }

  auto* dataBuffer = new char[BUFFER_SIZE];
  ALint buffersProcessed = 0;
  alGetSourcei(id_, AL_BUFFERS_PROCESSED, &buffersProcessed);
  if (buffersProcessed <= 0) {
    return;
  }
  while (buffersProcessed--) {
    ALuint bufferId;
    alSourceUnqueueBuffers(id_, 1, &bufferId);
    sound::Buffer* buffer = nullptr;
    for (auto* b : buffers_) {
      if (b->Id() == bufferId) {
        buffer = b;
      }
    }
    if (buffer == nullptr) {
      throw std::runtime_error("[StreamSource::Update]: buffer is null");
    }
    const auto size = cursor_->Read(dataBuffer, BUFFER_SIZE);
    buffer->SetData(dataBuffer, static_cast<int>(size),
                    AL_FORMAT_MONO8 + static_cast<int>(file_->Info().Format),
                    file_->Info().Samplerate);
    alSourceQueueBuffers(id_, 1, &bufferId);
  }
}

void Source::SetLooping(const bool doLoop) {
  if (loop_ == doLoop) {
    return;
  }
  loop_ = doLoop;
  if (buffers_.size() == 1) {
    if (doLoop) {
      alSourcei(this->id_, AL_LOOPING, AL_TRUE);
    } else {
      alSourcei(this->id_, AL_LOOPING, AL_FALSE);
    }
  } else if (cursor_ != nullptr) {
    cursor_->SetLoop(loop_);
  }
}

bool Source::GetLooping() const {
  ALint looping = 0;
  alGetSourcei(this->id_, AL_LOOPING, &looping);
  return looping == AL_TRUE;
}

void Source::SetSourceRelative(const bool relative) {
  alSourcei(this->id_, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
}

bool Source::GetSourceRelative() const {
  ALint relative = 0;
  alGetSourcei(this->id_, AL_SOURCE_RELATIVE, &relative);
  return relative == AL_TRUE;
}

void Source::Play() {
  alSourcePlay(this->id_);
  SetPlayState(PlayStateType::Playing);
}

void Source::Pause() {
  alSourcePause(this->id_);
  SetPlayState(PlayStateType::Stopped);
}

void Source::Rewind() { alSourceRewind(this->id_); }

void Source::Stop() {
  alSourceStop(this->id_);
  SetPlayState(PlayStateType::Stopped);
}

Source::PlayStateType Source::GetPlayState() const { return playState_; }

const File* Source::GetFile() const { return file_; }

float Source::GetMaxDistance() const {
  float distance = NAN;
  alGetSourcef(id_, AL_MAX_DISTANCE, &distance);
  return distance;
}

void Source::SetMaxDistance(const float distance) {
  alSourcef(id_, AL_MAX_DISTANCE, distance);
}

float Source::GetReferenceDistance() const {
  float distance = NAN;
  alGetSourcef(id_, AL_REFERENCE_DISTANCE, &distance);
  return distance;
}

void Source::SetReferenceDistance(const float distance) {
  alSourcef(id_, AL_REFERENCE_DISTANCE, distance);
}

float Source::GetRolloffFactor() const {
  float factor = NAN;
  alGetSourcef(id_, AL_ROLLOFF_FACTOR, &factor);
  return factor;
}

void Source::SetRolloffFactor(const float factor) {
  alSourcef(id_, AL_ROLLOFF_FACTOR, factor);
}

void Source::SetPlayState(const PlayStateType playState) {
  if (playState == playState_) {
    return;
  }
  this->playState_ = playState;
  fire(event::SourceEvent(event::SourceEvent::TriggerType::PlayStateChanged,
                          this));
}
}  // namespace soil::sound::openal
