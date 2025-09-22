
#ifndef SOIL_STAGE_SCENE_COMPONENT_SOUND_H
#define SOIL_STAGE_SCENE_COMPONENT_SOUND_H

#include "component.h"
#include "sound/source.h"

namespace soil::stage::scene::component {
    class Sound final : public Component, sound::event::EventHandler {
    public:
        /**
         * @param source to play
         * @param deleteAfterPlayed commit suicide after source finished to play
         */
        explicit Sound(sound::Source* source, bool deleteAfterPlayed = false);

        ~Sound() override;

        void Play() const;

        void Stop() const;

        [[nodiscard]] sound::Source& Source() const;

        void UpdateTransform(const glm::mat4& matrix) override;

        void Handle(const sound::event::Event& event) override;

    private:
        sound::Source* source_;
        bool deleteAfterPlayed_;
    };
} // namespace soil::stage::scene::component


#endif
