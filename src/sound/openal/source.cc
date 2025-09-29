#include "sound/openal/source.h"

#include <al.h>
#include <cmath>
#include <glm/vec3.hpp>

namespace soil::sound::openal {
    Source::Source(Buffer* buffer) :
        id_(0), buffer_(nullptr) {
        alGenSources(1, &id_);
        SetBuffer(buffer);
    }

    Source::~Source() {
        Observable<event::Event>::fire(
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

    void Source::SetPosition(glm::vec3 position) const {
        alSourcefv(this->id_, AL_POSITION, &position[0]);
    }

    float Source::GetPitch() const {
        float pitch = NAN;
        alGetSourcef(this->id_, AL_PITCH, &pitch);
        return pitch;
    }

    void Source::SetPitch(const float pitch) const {
        alSourcef(this->id_, AL_PITCH, pitch);
    }

    float Source::GetGain() const {
        float gain = NAN;
        alGetSourcef(this->id_, AL_GAIN, &gain);
        return gain;
    }

    void Source::SetGain(const float gain) const {
        alSourcef(this->id_, AL_GAIN, gain);
    }

    uint Source::GetId() const {
        return id_;
    }

    bool Source::IsPlaying() const {
        return GetPlayState() == PlayStateType::Playing;
    }

    void Source::UpdatePlayState() {
        ALint state = 0;
        alGetSourcei(this->id_, AL_SOURCE_STATE, &state);
        const auto newState = state == AL_PLAYING ? PlayStateType::Playing : PlayStateType::Stopped;
        if (newState == playState_) {
            return;
        }
        this->playState_ = newState;
        fire(
            event::SourceEvent(event::SourceEvent::TriggerType::PlayStateChanged, this));
    }

    void Source::SetLooping(const bool doLoop) const {
        if (doLoop) {
            alSourcei(this->id_, AL_LOOPING, AL_TRUE);
        } else {
            alSourcei(this->id_, AL_LOOPING, AL_FALSE);
        }
    }

    bool Source::GetLooping() const {
        ALint looping = 0;
        alGetSourcei(this->id_, AL_LOOPING, &looping);
        return looping == AL_TRUE;
    }

    void Source::SetSourceRelative(const bool relative) const {
        alSourcei(this->id_, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
    }

    bool Source::GetSourceRelative() const {
        ALint relative = 0;
        alGetSourcei(this->id_, AL_SOURCE_RELATIVE, &relative);
        return relative == AL_TRUE;
    }

    void Source::Play() {
        alSourcePlay(this->id_);
        playState_ = PlayStateType::Playing;
        fire(
            event::SourceEvent(event::SourceEvent::TriggerType::PlayStateChanged, this));
    }

    void Source::Pause() {
        alSourcePause(this->id_);
        playState_ = PlayStateType::Stopped;
        fire(
            event::SourceEvent(event::SourceEvent::TriggerType::PlayStateChanged, this));
    }

    void Source::Rewind() {
        alSourceRewind(this->id_);
    }

    void Source::Stop() {
        alSourceStop(this->id_);
        playState_ = PlayStateType::Stopped;
        fire(
            event::SourceEvent(event::SourceEvent::TriggerType::PlayStateChanged, this));
    }

    void Source::SetBuffer(Buffer* Buffer) {
        if (buffer_ == Buffer) {
            return;
        }
        buffer_ = Buffer;
        if (buffer_ != nullptr) {
            alSourcei(id_, AL_BUFFER, static_cast<ALint>(buffer_->getId()));
        } else {
            alSourcei(id_, AL_BUFFER, 0);
        }
    }

    Buffer* Source::GetBuffer() const {
        return buffer_;
    }

    Source::PlayStateType Source::GetPlayState() const {
        return playState_;
    }

    float Source::GetMaxDistance() const {
        float distance = NAN;
        alGetSourcef(id_, AL_MAX_DISTANCE, &distance);
        return distance;
    }

    void Source::SetMaxDistance(const float distance) const {
        alSourcef(id_, AL_MAX_DISTANCE, distance);
    }

    float Source::GetReferenceDistance() const {
        float distance = NAN;
        alGetSourcef(id_, AL_REFERENCE_DISTANCE, &distance);
        return distance;
    }

    void Source::SetReferenceDistance(const float distance) const {
        alSourcef(id_, AL_REFERENCE_DISTANCE, distance);
    }

    float Source::GetRolloffFactor() const {
        float factor = NAN;
        alGetSourcef(id_, AL_ROLLOFF_FACTOR, &factor);
        return factor;
    }

    void Source::SetRolloffFactor(const float factor) const {
        alSourcef(id_, AL_ROLLOFF_FACTOR, factor);
    }
} // namespace soil::sound::openal
