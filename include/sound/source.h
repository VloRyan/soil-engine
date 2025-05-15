#ifndef SOIL_SOUND_SOURCE_H
#define SOIL_SOUND_SOURCE_H
#include "glm/vec3.hpp"

#include "buffer.h"

namespace soil::sound {
    class Source {
    public:
        explicit Source() = default;

        virtual ~Source() = default;

        [[nodiscard]] virtual glm::vec3 GetPosition() const = 0;

        virtual void SetPosition(glm::vec3 position) const = 0;

        [[nodiscard]] virtual float GetPitch() const = 0;

        virtual void SetPitch(float) const = 0;

        [[nodiscard]] virtual float GetGain() const = 0;

        virtual void SetGain(float) const = 0;

        [[nodiscard]] virtual bool IsPlaying() const = 0;

        virtual void SetLooping(bool doLoop) const = 0;

        [[nodiscard]] virtual bool GetLooping() const = 0;

        virtual void SetSourceRelative(bool relative) const = 0;

        [[nodiscard]] virtual bool GetSourceRelative() const = 0;

        [[nodiscard]] virtual float GetMaxDistance() const = 0;

        virtual void SetMaxDistance(float distance) const = 0;

        [[nodiscard]] virtual float GetRolloffFactor() const = 0;

        virtual void SetRolloffFactor(float factor) const = 0;

        [[nodiscard]] virtual float GetReferenceDistance() const = 0;

        virtual void SetReferenceDistance(float distance) const = 0;

        virtual void Play() const = 0;

        virtual void Pause() const = 0;

        virtual void Rewind() const = 0;

        virtual void Stop() const = 0;

        virtual void SetBuffer(Buffer *Buffer) = 0;

        [[nodiscard]] virtual Buffer *GetBuffer() const = 0;
    };
} // namespace soil::sound
#endif // SOIL_SOUND_SOURCE_H
