#ifndef ENGINE_SOUND_SOURCE_H
#define ENGINE_SOUND_SOURCE_H
#include <glm/vec3.hpp>

#include "sound/buffer.h"

namespace sound {
    class Source {
    public:
        explicit Source(Buffer *buffer);

        virtual ~Source();


        [[nodiscard]] virtual glm::vec3 GetPosition() const;

        virtual void SetPosition(glm::vec3 position);

        [[nodiscard]] virtual float GetPitch() const;

        virtual void SetPitch(float);

        [[nodiscard]] virtual float GetGain() const;

        virtual void SetGain(float);

        [[nodiscard]] virtual bool IsPlaying() const;

        virtual void SetLooping(bool doLoop);

        [[nodiscard]] virtual bool GetLooping() const;

        virtual void SetSourceRelative(bool relative);

        [[nodiscard]] virtual bool GetSourceRelative() const;

        [[nodiscard]] virtual float GetMaxDistance() const;

        virtual void SetMaxDistance(float distance);

        [[nodiscard]] virtual float GetRolloffFactor() const;

        virtual void SetRolloffFactor(float factor);

        [[nodiscard]] virtual float GetReferenceDistance() const;

        virtual void SetReferenceDistance(float distance);

        virtual void Play();

        virtual void Pause();

        virtual void Rewind();

        virtual void Stop();

        void SetBuffer(Buffer *Buffer);

        [[nodiscard]] Buffer* GetBuffer() const;

    private:
        [[nodiscard]] uint GetId() const;

        uint id_;
        Buffer *buffer_;
    };
}
#endif // ENGINE_SOUND_SOURCE_H

