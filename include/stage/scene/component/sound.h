
#ifndef SOIL_STAGE_SCENE_COMPONENT_SOUND_H
#define SOIL_STAGE_SCENE_COMPONENT_SOUND_H

#include "component.h"
#include "sound/source.h"

namespace soil::stage::scene::component {
    class Sound final : public Component {
    public:
        explicit Sound(sound::Source *source);

        void Play() const;

        void Stop() const;

        [[nodiscard]] sound::Source *GetSource() const;

        void UpdateTransform(const glm::mat4 &matrix) override;

    private:
        sound::Source *source_;
    };
} // namespace soil::stage::scene::component


#endif
