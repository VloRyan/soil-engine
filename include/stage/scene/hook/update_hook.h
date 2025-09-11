#ifndef UPDATE_HOOK_H
#define UPDATE_HOOK_H
#include "hook.h"

namespace soil::stage::scene::hook {
    class UpdateHook : public Hook {
    public:
        explicit UpdateHook(HandlerType handlerType = HandlerType::None);
        ~UpdateHook() override = default;
        virtual void OnUpdate() = 0;
    };


} // namespace soil::stage::scene::hook


#endif
