#include "stage/scene/hook/hook.h"

namespace soil::stage::scene::hook {
Hook::Hook(const std::vector<Hook::Trigger_t>& triggers,
           const HandlerType handlerType)
    : triggers_(0), handlerType_(handlerType) {
  for (auto trigger : triggers) {
    triggers_[static_cast<int>(trigger)] = true;
  }
}

bool Hook::IsTrigger(Hook::Trigger_t trigger) {
  return triggers_[static_cast<int>(trigger)];
}

Hook::HandlerType Hook::GetHandlerType() const { return handlerType_; }

void Hook::Handle(const event::Component& event) {}
}  // namespace soil::stage::scene::hook
