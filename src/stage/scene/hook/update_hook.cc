#include "../../../../include/stage/scene/hook/update_hook.h"

namespace soil::stage::scene::hook {
UpdateHook::UpdateHook(const HandlerType handlerType)
    : Hook(Type::AfterUpdateScene, handlerType) {}
}  // namespace soil::stage::scene::hook
