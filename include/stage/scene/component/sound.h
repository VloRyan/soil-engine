
#ifndef ENGINE_STAGE_SCENE_COMPONENT_SOUND_H
#define ENGINE_STAGE_SCENE_COMPONENT_SOUND_H

#include "component.h"
#include "../../../sound/source.h"

namespace stage::scene::component {
    class Sound : public Component {
    public:
        explicit Sound(::sound::Source *source);

        void Play() const;

        void Stop() const;

        [[nodiscard]] ::sound::Source* GetSource() const;

        void UpdateMatrix(const glm::mat4 &matrix) override;

    private:
        ::sound::Source *source_;
    };
}


#endif //ENGINE_STAGE_SCENE_COMPONENT_SOUND_H
