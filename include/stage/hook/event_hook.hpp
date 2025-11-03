#ifndef SOIL_STAGE_HOOK_EVENT_HOOK_H
#define SOIL_STAGE_HOOK_EVENT_HOOK_H

#include <type_traits>

#include "event/event.hpp"
namespace soil::stage::hook {
template <class T>
class EventHook {
 public:
  EventHook() {
    using type = std::remove_pointer_t<T>;
    static_assert(std::is_base_of_v<soil::event::Event, type>,
                  "node must be derived from soil::event::Event");
  }
  virtual ~EventHook() = default;
  virtual void OnEvent(const T& event) = 0;
};
}  // namespace soil::stage::hook

#endif
