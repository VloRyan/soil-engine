#ifndef RENDER_HOOK_H
#define RENDER_HOOK_H
#include "hook.h"

namespace soil::stage::scene::hook {
    class RenderHook : public Hook {
    public:
        explicit RenderHook(HandlerType handlerType);
        ~RenderHook() override = default;
        virtual void OnRender(video::render::State& state) = 0;
    };
} // namespace soil::stage::scene::hook


#endif
