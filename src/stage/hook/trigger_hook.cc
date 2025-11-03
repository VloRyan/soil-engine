#include "stage/hook/trigger_hook.h"

namespace soil::stage::hook {
TriggerHook::TriggerHook(const std::vector<TriggerType>& triggers)
    : triggers_(0) {
  SetTriggers(triggers);
}

bool TriggerHook::IsTrigger(TriggerType trigger) {
  return triggers_[static_cast<int>(trigger)];
}
void TriggerHook::SetTriggers(const std::vector<TriggerType>& triggers) {
  for (auto trigger : triggers) {
    triggers_[static_cast<int>(trigger)] = true;
  }
}
}  // namespace soil::stage::hook
