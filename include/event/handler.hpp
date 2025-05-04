#ifndef ENGINE_EVENT_HANDLER_H
#define ENGINE_EVENT_HANDLER_H
#include <type_traits>
#include "event/event.h"

namespace event
{
    template <class T, std::enable_if_t<std::is_base_of_v<Event, T>> * = nullptr>
    class Handler
    {
    public:
        Handler() = default;
        virtual ~Handler() = default;
        virtual void Handle(const T &event) = 0;
    };
} // namespace event
#endif /* ENGINE_EVENT_HANDLER_H */
