#ifndef ENGINE_EVENT_HANDLER_H
#define ENGINE_EVENT_HANDLER_H
#include <functional>
#include <type_traits>
#include "event/event.h"

namespace soil::event {
    template <class T, std::enable_if_t<std::is_base_of_v<Event, T>>* = nullptr>
    class Observable;

    template <class T, std::enable_if_t<std::is_base_of_v<Event, T>> * = nullptr>
    class Handler {
    public:
        Handler() = default;

        virtual ~Handler() {
            if (onDelete_ != nullptr) {
                onDelete_(this);
            }
        }

        virtual void Handle(const T& event) = 0;

        friend class Observable<T>;

    private:
        std::function<void(Handler<T>* handler)> onDelete_ = nullptr;
    };
} // namespace soil::event
#endif /* ENGINE_EVENT_HANDLER_H */
