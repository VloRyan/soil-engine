
#ifndef SOIL_STAGE_SCENE_COMPONENT_LISTENER_COMPONENT_H
#define SOIL_STAGE_SCENE_COMPONENT_LISTENER_COMPONENT_H
#include "component.h"
#include "sound/listener.h"

namespace soil::stage::scene::component {
    class ListenerComponent final : public Component {
    public:
        explicit ListenerComponent(sound::Listener* listener, bool setOrientation = false);

        ~ListenerComponent() override = default;

        [[nodiscard]] sound::Listener* GetListener() const;

        void UpdateTransform(const glm::mat4& matrix) override;

    private:
        sound::Listener* listener_;
        bool setOrientation_;
    };
} // namespace soil::stage::scene::component

#endif
