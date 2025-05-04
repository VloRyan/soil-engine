
#ifndef ENGINE_STAGE_SCENE_COMPONENT_LISTENER_H
#define ENGINE_STAGE_SCENE_COMPONENT_LISTENER_H
#include "component.h"
#include "sound/listener.h"

namespace stage::scene::component {
    class Listener final : public Component {
    public:
        explicit Listener(::sound::Listener *listener);

        ~Listener() override = default;

        [[nodiscard]] ::sound::Listener* GetListener() const;

        void UpdateMatrix(const glm::mat4 &matrix) override;

    protected:


    private:
        ::sound::Listener *listener_;
    };
}

#endif //ENGINE_STAGE_SCENE_COMPONENT_LISTENER_H
