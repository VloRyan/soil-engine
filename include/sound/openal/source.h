#ifndef SOIL_SOUND_OPENAL_SOURCE_H
#define SOIL_SOUND_OPENAL_SOURCE_H
#include "sound/source.h"

namespace soil::sound::openal {
    class Source final : public sound::Source {
    public:
        explicit Source(sound::Buffer *buffer);

        ~Source() override;

        [[nodiscard]] glm::vec3 GetPosition() const override;

        void SetPosition(glm::vec3 position) const override;

        [[nodiscard]] float GetPitch() const override;

        void SetPitch(float) const override;

        [[nodiscard]] float GetGain() const override;

        void SetGain(float) const override;

        [[nodiscard]] bool IsPlaying() const override;

        void SetLooping(bool doLoop) const override;

        [[nodiscard]] bool GetLooping() const override;

        void SetSourceRelative(bool relative) const override;

        [[nodiscard]] bool GetSourceRelative() const override;

        [[nodiscard]] float GetMaxDistance() const override;

        void SetMaxDistance(float distance) const override;

        [[nodiscard]] float GetRolloffFactor() const override;

        void SetRolloffFactor(float factor) const override;

        [[nodiscard]] float GetReferenceDistance() const override;

        void SetReferenceDistance(float distance) const override;

        void Play() const override;

        void Pause() const override;

        void Rewind() const override;

        void Stop() const override;

        void SetBuffer(sound::Buffer *Buffer) override;

        [[nodiscard]] sound::Buffer *GetBuffer() const override;

    private:
        [[nodiscard]] uint GetId() const;

        uint id_;
        Buffer *buffer_;
    };
} // namespace soil::sound::openal
#endif // SOIL_SOUND_OPENAL_SOURCE_H
