#include "stage/scene/hook/hook.h"

namespace soil::stage::scene::hook {
Hook::Hook(const Type type, const HandlerType handlerType)
    : type_(type), handlerType_(handlerType) {}

Type Hook::GetType() const { return type_; }

Hook::HandlerType Hook::GetHandlerType() const { return handlerType_; }

void Hook::Handle(const event::Component& event) {}
}  // namespace soil::stage::scene::hook
