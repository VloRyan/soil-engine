
#ifndef SOIL_STAGE_SCENE_COMPONENT_LISTENER_H
#define SOIL_STAGE_SCENE_COMPONENT_LISTENER_H
#include "component.h"
#include "sound/listener.h"

namespace soil::stage::scene::component {
    class Listener final : public Component {
    public:
        explicit Listener(sound::Listener *listener);

        ~Listener() override = default;

        [[nodiscard]] sound::Listener *GetListener() const;

        void UpdateTransform(const glm::mat4 &matrix) override;

    private:
        sound::Listener *listener_;
    };
} // namespace soil::stage::scene::component

#endif
