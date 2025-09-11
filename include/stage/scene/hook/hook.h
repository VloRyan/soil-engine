#ifndef SOIL_STAGE_SCENE_HOOK_HOOK_H
#define SOIL_STAGE_SCENE_HOOK_HOOK_H
#include "stage/event/component.h"
#include "video/render/container.h"

namespace soil::stage::scene::hook {
    enum class Type { AfterUpdateScene, Render };

    class Hook : public event::ComponentEventHandler {
    public:
        enum class HandlerType : std::uint8_t { None = 0, Component };
        ~Hook() override = default;
        [[nodiscard]] Type GetType() const;
        [[nodiscard]] virtual HandlerType GetHandlerType() const;
        void Handle(const event::Component& event) override;

    protected:
        explicit Hook(Type type, HandlerType handlerType = HandlerType::None);

    private:
        Type type_;
        HandlerType handlerType_;
    };
} // namespace soil::stage::scene::hook

#endif
