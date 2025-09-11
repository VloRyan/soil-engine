#include "stage/scene/hook/render_hook.h"

namespace soil::stage::scene::hook {
    RenderHook::RenderHook(const HandlerType handlerType) : Hook(Type::Render, handlerType) {}
} // namespace soil::stage::scene::hook
